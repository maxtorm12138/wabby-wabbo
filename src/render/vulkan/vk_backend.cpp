// backend interface
#include "wabby/render/backend.hpp"

// module
#include "vk_allocator.hpp"
#include "vk_defines.hpp"
#include "vk_device_allocator.hpp"
#include "vk_environment.hpp"
#include "vk_framebuffers.hpp"
#include "vk_hardware.hpp"
#include "vk_render_pass.hpp"
#include "vk_swapchian.hpp"

// inipp
#include "inipp.h"

// boost

// spdlog
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

// common include
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{
  class vk_backend : public boost::noncopyable
  {
  public:
    vk_backend() {}

    ~vk_backend() {}

    int setup( const vk_backend_setup_info * setup_info );

    int begin_frame();

    int end_frame();

    int begin_render_pass();

    int end_render_pass();

    int resized();

    int teardown();

  private:
    wabby::container::delayed<vk_environment>       environment_;
    wabby::container::delayed<vk::raii::SurfaceKHR> surface_;
    wabby::container::delayed<vk_hardware>          hardware_;
    wabby::container::delayed<vk_device_allocator>  device_allocator_;
    wabby::container::delayed<vk_swapchain>         swapchain_;
    wabby::container::delayed<vk_render_pass>       render_pass_;
    wabby::container::delayed<vk_framebuffers>      framebuffers_;
    uint32_t                                        image_index_;
    uint64_t                                        frame_index_;
    vk_vector<vk::raii::CommandBuffer>              command_buffers_;
    vk_vector<vk::raii::Semaphore>                  image_available_semaphores_;
    vk_vector<vk::raii::Semaphore>                  render_finished_semaphores_;
    vk_vector<vk::raii::Fence>                      in_flight_fences_;
  };
}  // namespace wabby::render::vulkan

#define REGISTER_PROC_ADDR( PFN, FN )                 \
  {                                                   \
#    FN, reinterpret_cast < pfn_void_function>( &FN ) \
  }

extern "C"
{
  int backend_setup( backend handle, const void * setup_info )
  {
    return static_cast<wabby::render::vulkan::vk_backend *>( handle )->setup( reinterpret_cast<const vk_backend_setup_info *>( setup_info ) );
  }

  int backend_teardown( backend handle )
  {
    return static_cast<wabby::render::vulkan::vk_backend *>( handle )->teardown();
  }

  int backend_begin_frame( backend handle )
  {
    return static_cast<wabby::render::vulkan::vk_backend *>( handle )->begin_frame();
  }

  int backend_end_frame( backend handle )
  {
    return static_cast<wabby::render::vulkan::vk_backend *>( handle )->end_frame();
  }

  int backend_begin_render_pass( backend handle )
  {
    return static_cast<wabby::render::vulkan::vk_backend *>( handle )->begin_render_pass();
  }

  int backend_end_render_pass( backend handle )
  {
    return static_cast<wabby::render::vulkan::vk_backend *>( handle )->end_render_pass();
  }

  int backend_resized( backend handle )
  {
    return static_cast<wabby::render::vulkan::vk_backend *>( handle )->resized();
  }

  int create_backend( backend * p_handle )
  {
    using namespace wabby::render::vulkan;
    auto p_backend = static_cast<vk_backend *>( global::allocation( sizeof( vk_backend ), 0 ) );
    std::construct_at( p_backend );
    *p_handle = p_backend;
    return 0;
  }

  void destroy_backend( backend handle )
  {
    using namespace wabby::render::vulkan;
    auto p_backend = static_cast<vk_backend *>( handle );
    std::destroy_at( p_backend );
    global::free( p_backend );
  }

  void set_backend_allocator( backend_allocator allocator )
  {
    using namespace wabby::render::vulkan;
    global::allocation    = fn_allocation( allocator->fn_allocation, allocator->user_args );
    global::realllocation = fn_reallocation( allocator->fn_reallocation, allocator->user_args );
    global::free          = fn_free( allocator->fn_free, allocator->user_args );
  }

  pfn_void_function get_proc_addr( const char * name )
  {
    static std::pair<const char *, pfn_void_function> addresses[] = {
      REGISTER_PROC_ADDR( pfn_backend_setup, backend_setup ),
      REGISTER_PROC_ADDR( pfn_backend_teardown, backend_teardown ),
      REGISTER_PROC_ADDR( pfn_set_backend_allocator, set_backend_allocator ),
      REGISTER_PROC_ADDR( pfn_destroy_backend, destroy_backend ),
      REGISTER_PROC_ADDR( pfn_create_backend, create_backend ),
      REGISTER_PROC_ADDR( pfn_backend_resized, backend_resized ),
      REGISTER_PROC_ADDR( pfn_backend_end_render_pass, backend_end_render_pass ),
      REGISTER_PROC_ADDR( pfn_backend_begin_render_pass, backend_begin_render_pass ),
      REGISTER_PROC_ADDR( pfn_backend_end_frame, backend_end_frame ),
      REGISTER_PROC_ADDR( pfn_backend_begin_frame, backend_begin_frame ),
    };

    for ( size_t i = 0; i < sizeof( addresses ) / sizeof( std::pair<const char *, void *> ); i++ )
    {
      if ( strcmp( name, addresses[i].first ) == 0 )
      {
        return addresses[i].second;
      }
    }
    return nullptr;
  }
}

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

  vk_vector<vk::ArrayProxy<const vk::ImageView>> build_fb_attachments( const vk_swapchain & swapchain )
  {
    vk_vector<vk::ArrayProxy<const vk::ImageView>> attachments;
    attachments.reserve( swapchain.image_count() );

    for ( auto & image_view : swapchain.image_views() )
    {
      vk::ArrayProxy<const vk::ImageView> attachment( *image_view );
      attachments.emplace_back( std::move( attachment ) );
    }

    return attachments;
  }

  vk_vector<vk::raii::Semaphore> build_semaphores( const vk::raii::Device & device, uint32_t size )
  {
    vk_vector<vk::raii::Semaphore> semaphores;
    semaphores.reserve( size );

    vk::SemaphoreCreateInfo semaphore_create_info{};
    for ( uint32_t i = 0; i < size; i++ )
    {
      semaphores.emplace_back( device, semaphore_create_info );
    }
    return semaphores;
  }

  vk_vector<vk::raii::Fence> build_fences( const vk::raii::Device & device, uint32_t size, bool signaled = true )
  {
    vk_vector<vk::raii::Fence> fences;
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

  spdlog::logger build_logger( inipp::Ini<char> & config )
  {
    std::string                                       level{ "off" };
    uint32_t                                          sinks_count{ 0 };
    std::vector<std::shared_ptr<spdlog::sinks::sink>> sinks;
    inipp::get_value( config.sections["log"], "level", level );
    inipp::get_value( config.sections["log"], "sink_count", sinks_count );

    for ( uint32_t i = 0; i < sinks_count; i++ )
    {
      std::string log_sink_attribute;
      inipp::get_value( config.sections["log"], fmt::format( "sink_{}", i ), log_sink_attribute );

      std::vector<std::string> attributes;
      boost::split( attributes, log_sink_attribute, boost::is_any_of( ":" ) );
      if ( attributes[0] == "file" )
      {
        sinks.emplace_back( std::make_shared<spdlog::sinks::basic_file_sink_mt>( attributes[1], true ) );
      }
      else if ( attributes[0] == "console" )
      {
        if ( attributes[1] == "stderr" )
        {
          sinks.emplace_back( std::make_shared<spdlog::sinks::stderr_color_sink_mt>() );
        }
        else if ( attributes[1] == "stdout" )
        {
          sinks.emplace_back( std::make_shared<spdlog::sinks::stdout_color_sink_mt>() );
        }
      }
    }

    spdlog::logger logger( "vulkan", sinks.begin(), sinks.end() );
    logger.set_level( spdlog::level::from_str( level ) );
    return logger;
  }

  int vk_backend::setup( const vk_backend_setup_info * setup_info )
  {
    try
    {
      std::ifstream    config_file( setup_info->configuration_path );
      inipp::Ini<char> config;
      config.parse( config_file );
      config.strip_trailing_comments();

      global::logger.construct( build_logger( config ) );

      environment_.construct( build_application_info( setup_info ), setup_info->windowsystem_extensions, setup_info->windowsystem_extensions_count );
      surface_.construct( environment_->instance(), setup_info->fn_vk_create_surface( setup_info->user_args, *environment_->instance() ) );
      hardware_.construct( environment_->instance(), *surface_ );
      device_allocator_.construct( environment_->instance(), hardware_->physical_device(), hardware_->device() );

      auto fn_get_window_size           = setup_info->fn_get_window_size;
      auto fn_get_window_size_user_args = setup_info->user_args;

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
    catch ( vk::Error & e )
    {
      SPDLOG_LOGGER_ERROR( global::logger, "setup: vk::Error, what: {}", e.what() );
      return 1;
    }
    catch ( std::exception & e )
    {
      SPDLOG_LOGGER_ERROR( global::logger, "setup: std::exception, what: {}", e.what() );
      return 2;
    }
    catch ( ... )
    {
      SPDLOG_LOGGER_ERROR( global::logger, "setup: unknwon exception" );
      return 3;
    }
    return 0;
  }

  int vk_backend::begin_frame()
  {
    try
    {
      hardware_->device().waitForFences( *in_flight_fences_[frame_index_], VK_TRUE, UINT64_MAX );
      image_index_ = swapchain_->acquire_next_image( image_available_semaphores_[frame_index_] );
      hardware_->device().resetFences( *in_flight_fences_[frame_index_] );

      vk::CommandBufferBeginInfo command_buffer_begin_info{};
      command_buffers_[frame_index_].begin( command_buffer_begin_info );
    }
    catch ( ... )
    {
      return -1;
    }
    return 0;
  }

  int vk_backend::end_frame()
  {
    try
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
    catch ( ... )
    {
      return -1;
    }
    return 0;
  }

  int vk_backend::begin_render_pass()
  {
    try
    {
      vk::Rect2D     render_area{ .offset = { 0, 0 }, .extent = swapchain_->extent() };
      vk::ClearValue clear_value{};
      clear_value.color.float32 = std::array<float, 4>{ 0.1f, 0.6f, 0.1f, 0.1f };

      render_pass_->begin( command_buffers_[frame_index_], framebuffers_->framebuffer( image_index_ ), render_area, clear_value );
    }
    catch ( ... )
    {
      return -1;
    }
    return 0;
  }

  int vk_backend::end_render_pass()
  {
    try
    {
      render_pass_->end( command_buffers_[frame_index_] );
    }
    catch ( ... )
    {
      return -1;
    }
    return 0;
  }

  int vk_backend::resized()
  {
    return 0;
  }

  int vk_backend::teardown()
  {
    try
    {
      hardware_->device().waitIdle();
    }
    catch ( ... )
    {
      return -1;
    }
    return 0;
  }

}  // namespace wabby::render::vulkan
