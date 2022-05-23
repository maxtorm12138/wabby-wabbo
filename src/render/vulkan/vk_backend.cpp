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

  vk_backend_context::vk_backend_context( const vk_backend_setup_info * setup_info )
    : environment_( build_application_info( setup_info ), setup_info->windowsystem_extensions, setup_info->windowsystem_extensions_count )
    , surface_( environment_.instance(), setup_info->fn_make_surface( *environment_.instance(), setup_info->fn_make_surface_user_args ) )
    , hardware_( environment_.instance(), surface_ )
    , device_allocator_( environment_.instance(), hardware_.physical_device(), hardware_.device() )
    , swapchain_( hardware_, surface_, setup_info->fn_get_window_size( setup_info->fn_get_window_size_user_args ) )
    , render_pass_( hardware_.device(), swapchain_.surface_format() )
    , image_index_( -1 )
    , frame_index_( 0 )
    , framebuffers_( hardware_.device(), render_pass_.render_pass(), swapchain_.image_count(), build_fb_attachments( swapchain_ ), swapchain_.extent() )
    , command_buffers_( hardware_.allocate_graphics_command_buffers( swapchain_.max_frames_in_flight() ) )
    , image_available_semaphores_( build_semaphores( hardware_.device(), swapchain_.max_frames_in_flight() ) )
    , render_finished_semaphores_( build_semaphores( hardware_.device(), swapchain_.max_frames_in_flight() ) )
    , in_flight_fences_( build_fences( hardware_.device(), swapchain_.max_frames_in_flight() ) )
  {
  }

  void vk_backend::setup( const vk_backend_setup_info * setup_info ) {}

  void vk_backend::begin_frame()
  {
    ctx_->hardware_.device().waitForFences( *ctx_->in_flight_fences_[ctx_->frame_index_], VK_TRUE, UINT64_MAX );
    ctx_->image_index_ = ctx_->swapchain_.acquire_next_image( ctx_->image_available_semaphores_[ctx_->frame_index_] );
    ctx_->hardware_.device().resetFences( *ctx_->in_flight_fences_[ctx_->frame_index_] );

    vk::CommandBufferBeginInfo command_buffer_begin_info{};
    ctx_->command_buffers_[ctx_->frame_index_].begin( command_buffer_begin_info );
  }

  void vk_backend::end_frame()
  {
    ctx_->command_buffers_[ctx_->frame_index_].end();
    vk::ArrayProxy<const vk::Semaphore>     submit_wait_semaphores( *ctx_->image_available_semaphores_[ctx_->frame_index_] );
    vk::ArrayProxy<const vk::Semaphore>     submit_signal_semaphores( *ctx_->render_finished_semaphores_[ctx_->frame_index_] );
    vk::ArrayProxy<const vk::CommandBuffer> submit_command_buffers( *ctx_->command_buffers_[ctx_->frame_index_] );
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

    ctx_->hardware_.queue( QueueType::GRAPHICS )->submit( submit_info, *ctx_->in_flight_fences_[ctx_->frame_index_] );

    vk::ArrayProxy<const vk::Semaphore>    presnet_wait_semaphores( *ctx_->render_finished_semaphores_[ctx_->frame_index_] );
    vk::ArrayProxy<const vk::SwapchainKHR> present_swapchains( *ctx_->swapchain_.swaichain() );

    vk::PresentInfoKHR present_info{ .waitSemaphoreCount = presnet_wait_semaphores.size(),
                                     .pWaitSemaphores    = presnet_wait_semaphores.data(),
                                     .swapchainCount     = present_swapchains.size(),
                                     .pSwapchains        = present_swapchains.data(),
                                     .pImageIndices      = &ctx_->image_index_ };

    ctx_->hardware_.queue( QueueType::PRESENT, std::cref( ctx_->surface_ ) )->presentKHR( present_info );

    ctx_->frame_index_ = ( ctx_->frame_index_ + 1 ) % ctx_->swapchain_.max_frames_in_flight();
  }

  void vk_backend::begin_render_pass()
  {
    vk::Rect2D     render_area{ .offset = { 0, 0 }, .extent = ctx_->swapchain_.extent() };
    vk::ClearValue clear_value{};
    clear_value.color.float32 = std::array<float, 4>{ 0.1f, 0.6f, 0.1f, 0.1f };

    ctx_->render_pass_.begin( ctx_->command_buffers_[ctx_->frame_index_], ctx_->framebuffers_.framebuffer( ctx_->image_index_ ), render_area, clear_value );
  }

  void vk_backend::end_render_pass()
  {
    ctx_->render_pass_.end( ctx_->command_buffers_[ctx_->frame_index_] );
  }

  void vk_backend::resized() {}

  void vk_backend::teardown()
  {
    ctx_->hardware_.device().waitIdle();
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
      auto vk_backend = new struct backend_t;

      vk_backend->internal_handle   = new wabby::render::vulkan::vk_backend;
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
      delete backend;
    }
  }
}  // namespace wabby::render::vulkan::detail
