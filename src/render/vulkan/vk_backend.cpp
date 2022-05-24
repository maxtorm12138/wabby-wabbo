#include "vk_backend.hpp"

namespace wabby::render::vulkan
{

  vk::ApplicationInfo build_application_info( const vk_backend_setup_info * setup_info )
  {
    vk::ApplicationInfo app_info{ .pApplicationName   = setup_info->application_name,
                                  .applicationVersion = setup_info->application_version,
                                  .pEngineName        = WABBY_ENGINE_NAME.data(),
                                  .engineVersion      = VK_MAKE_VERSION( 1, 0, 0 ),
                                  .apiVersion         = VK_API_VERSION_1_1 };

    return app_info;
  }

  std::vector<vk::ArrayProxy<const vk::ImageView>> build_fb_attachments( const vk_swapchain & swapchain )
  {
    std::vector<vk::ArrayProxy<const vk::ImageView>> attachments;
    attachments.reserve( swapchain.image_count() );

    for ( auto & image_view : swapchain.image_views() )
    {
      vk::ArrayProxy<const vk::ImageView> attachment( *image_view );
      attachments.emplace_back( std::move( attachment ) );
    }

    return attachments;
  }

  std::vector<vk::raii::Semaphore> build_semaphores( const vk::raii::Device & device, uint32_t size )
  {
    std::vector<vk::raii::Semaphore> semaphores;
    semaphores.reserve( size );

    vk::SemaphoreCreateInfo semaphore_create_info{};
    for ( uint32_t i = 0; i < size; i++ )
    {
      semaphores.emplace_back( device, semaphore_create_info );
    }
    return semaphores;
  }

  std::vector<vk::raii::Fence> build_fences( const vk::raii::Device & device, uint32_t size, bool signaled = true )
  {
    std::vector<vk::raii::Fence> fences;
    fences.reserve( size );

    vk::FenceCreateFlags fence_create_flags;
    if ( signaled )
    {
      fence_create_flags |= vk::FenceCreateFlagBits::eSignaled;
    }

    vk::FenceCreateInfo fence_create_info{ .flags = fence_create_flags };
    for ( uint32_t i = 0; i < size; i++ )
    {
      fences.emplace_back( device, fence_create_info );
    }

    return fences;
  }

  void vk_backend::setup( const vk_backend_setup_info * setup_info )
  {
    environment_.construct( build_application_info( setup_info ), setup_info->windowsystem_extensions, setup_info->windowsystem_extensions_count );
    surface_.construct( environment_->instance(), setup_info->fn_make_surface( setup_info->fn_make_surface_user_args, *environment_->instance() ) );
    hardware_.construct( environment_->instance(), *surface_ );
    device_allocator_.construct( environment_->instance(), hardware_->physical_device(), hardware_->device() );

    auto fn_get_window_size           = setup_info->fn_get_window_size;
    auto fn_get_window_size_user_args = setup_info->fn_make_surface_user_args;

    auto fn_get_window_size_wrapper = [fn_get_window_size_user_args, fn_get_window_size]()
    {
      std::pair<uint32_t, uint32_t> result;
      fn_get_window_size( fn_get_window_size_user_args, &result.first, &result.second );
      return result;
    };

    swapchain_.construct( hardware_, *surface_, fn_get_window_size_wrapper );
    render_pass_.construct( hardware_->device(), swapchain_->surface_format() );
    framebuffers_.construct(
      hardware_->device(), render_pass_->render_pass(), swapchain_->image_count(), build_fb_attachments( *swapchain_ ), swapchain_->extent() );

    image_index_ = 0;
    frame_index_ = 0;

    command_buffers_            = hardware_->allocate_graphics_command_buffers( swapchain_->max_frames_in_flight() );
    image_available_semaphores_ = build_semaphores( hardware_->device(), swapchain_->max_frames_in_flight() );
    render_finished_semaphores_ = build_semaphores( hardware_->device(), swapchain_->max_frames_in_flight() );
    in_flight_fences_           = build_fences( hardware_->device(), swapchain_->max_frames_in_flight() );
  }

  void vk_backend::begin_frame()
  {
    hardware_->device().waitForFences( *in_flight_fences_[frame_index_], VK_TRUE, UINT64_MAX );
    image_index_ = swapchain_->acquire_next_image( image_available_semaphores_[frame_index_] );
    hardware_->device().resetFences( *in_flight_fences_[frame_index_] );

    vk::CommandBufferBeginInfo command_buffer_begin_info{};
    command_buffers_[frame_index_].begin( command_buffer_begin_info );
  }

  void vk_backend::end_frame()
  {
    command_buffers_[frame_index_].end();
    vk::ArrayProxy<const vk::Semaphore>     submit_wait_semaphores( *image_available_semaphores_[frame_index_] );
    vk::ArrayProxy<const vk::Semaphore>     submit_signal_semaphores( *render_finished_semaphores_[frame_index_] );
    vk::ArrayProxy<const vk::CommandBuffer> submit_command_buffers( *command_buffers_[frame_index_] );
    std::array<vk::PipelineStageFlags, 1>   wait_dst_stages{ vk::PipelineStageFlagBits::eColorAttachmentOutput };

    vk::SubmitInfo submit_info{
      .waitSemaphoreCount   = submit_wait_semaphores.size(),
      .pWaitSemaphores      = submit_wait_semaphores.data(),
      .pWaitDstStageMask    = wait_dst_stages.data(),
      .commandBufferCount   = submit_command_buffers.size(),
      .pCommandBuffers      = submit_command_buffers.data(),
      .signalSemaphoreCount = submit_signal_semaphores.size(),
      .pSignalSemaphores    = submit_signal_semaphores.data(),
    };

    hardware_->queue( QueueType::GRAPHICS )->submit( submit_info, *in_flight_fences_[frame_index_] );

    vk::ArrayProxy<const vk::Semaphore>    presnet_wait_semaphores( *render_finished_semaphores_[frame_index_] );
    vk::ArrayProxy<const vk::SwapchainKHR> present_swapchains( *swapchain_->swaichain() );

    vk::PresentInfoKHR present_info{ .waitSemaphoreCount = presnet_wait_semaphores.size(),
                                     .pWaitSemaphores    = presnet_wait_semaphores.data(),
                                     .swapchainCount     = present_swapchains.size(),
                                     .pSwapchains        = present_swapchains.data(),
                                     .pImageIndices      = &image_index_ };

    hardware_->queue( QueueType::PRESENT, std::cref( *surface_ ) )->presentKHR( present_info );

    frame_index_ = ( frame_index_ + 1 ) % swapchain_->max_frames_in_flight();
  }

  void vk_backend::begin_render_pass()
  {
    vk::Rect2D     render_area{ .offset = { 0, 0 }, .extent = swapchain_->extent() };
    vk::ClearValue clear_value{};
    clear_value.color.float32 = std::array<float, 4>{ 0.1f, 0.6f, 0.1f, 0.1f };

    render_pass_->begin( command_buffers_[frame_index_], framebuffers_->framebuffer( image_index_ ), render_area, clear_value );
  }

  void vk_backend::end_render_pass()
  {
    render_pass_->end( command_buffers_[frame_index_] );
  }

  void vk_backend::resized() {}

  void vk_backend::teardown()
  {
    hardware_->device().waitIdle();
  }

}  // namespace wabby::render::vulkan

namespace wabby::render::vulkan::detail
{

  void setup( backend_t * handle, const backend_setup_info * setup_info )
  {
    static_cast<wabby::render::vulkan::vk_backend *>( handle->internal_handle )->setup( reinterpret_cast<const vk_backend_setup_info *>( setup_info ) );
  }

  void begin_frame( backend_t * handle )
  {
    static_cast<wabby::render::vulkan::vk_backend *>( handle->internal_handle )->begin_frame();
  }

  void end_frame( backend_t * handle )
  {
    static_cast<wabby::render::vulkan::vk_backend *>( handle->internal_handle )->end_frame();
  }

  void begin_render_pass( backend_t * handle )
  {
    static_cast<wabby::render::vulkan::vk_backend *>( handle->internal_handle )->begin_render_pass();
  }

  void end_render_pass( backend_t * handle )
  {
    static_cast<wabby::render::vulkan::vk_backend *>( handle->internal_handle )->end_render_pass();
  }

  void resized( backend_t * handle )
  {
    static_cast<wabby::render::vulkan::vk_backend *>( handle->internal_handle )->resized();
  }

  void teardown( backend_t * handle )
  {
    static_cast<wabby::render::vulkan::vk_backend *>( handle->internal_handle )->teardown();
  }

  extern "C"
  {
    int create_backend( backend * backend )
    {
      vk_allocator<backend_t>  allocator;
      vk_allocator<vk_backend> vk_backend_allocator;

      auto vk_backend      = allocator.allocate( 1 );
      auto vk_backend_impl = vk_backend_allocator.allocate( 1 );
      std::construct_at( vk_backend_impl );

      vk_backend->internal_handle   = vk_backend_impl;
      vk_backend->setup             = setup;
      vk_backend->teardown          = teardown;
      vk_backend->begin_frame       = begin_frame;
      vk_backend->end_frame         = end_frame;
      vk_backend->begin_render_pass = begin_render_pass;
      vk_backend->end_render_pass   = end_render_pass;
      vk_backend->resized           = resized;

      *backend = vk_backend;
      return 0;
    }

    void destroy_backend( backend backend )
    {
      vk_allocator<backend_t>  allocator;
      vk_allocator<vk_backend> vk_backend_allocator;

      std::destroy_at( static_cast<vk_backend *>( backend->internal_handle ) );

      vk_backend_allocator.deallocate( static_cast<vk_backend *>( backend->internal_handle ), 1 );
      allocator.deallocate( backend, 1 );
    }

    void set_allocation_callbacks( const allocation_callbacks * allocation_callbacks )
    {
      detail::vk_allocator_impl::setup(
        allocation_callbacks->user_args, allocation_callbacks->allocation, allocation_callbacks->reallocation, allocation_callbacks->free );
    }
  }
}  // namespace wabby::render::vulkan::detail
