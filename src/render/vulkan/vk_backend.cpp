#include "vk_backend.hpp"

// inipp
#include "inipp.h"

// spdlog
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

// common include
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{
  vk::ApplicationInfo build_application_info( const vk_backend_setup_info * setup_info )
  {
    vk::ApplicationInfo app_info{ .pApplicationName   = setup_info->application_name,
                                  .applicationVersion = setup_info->application_version,
                                  .pEngineName        = WABBY_ENGINE_NAME.data(),
                                  .engineVersion      = WABBY_ENGINE_VERSION,
                                  .apiVersion         = WABBY_ENGINE_VULKAN_VERSION };
    return app_info;
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
      surface_.construct( environment_->instance(), setup_info->fn_vk_create_surface, setup_info->user_args );
      hardware_.construct( environment_->instance(), *surface_ );
      queue_cache_.construct( hardware_, surface_ );

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
      framebuffers_.construct( hardware_->device(), render_pass_->render_pass(), swapchain_ );

      image_index_ = 0;
      frame_index_ = 0;

      command_buffers_.construct( hardware_->device(), *graphics_command_pool_, swapchain_->max_frames_in_flight() );
      image_available_semaphores_.construct( hardware_->device(), swapchain_->max_frames_in_flight() );
      render_finished_semaphores_.construct( hardware_->device(), swapchain_->max_frames_in_flight() );
      in_flight_fences_.construct( hardware_->device(), swapchain_->max_frames_in_flight() );
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
      hardware_->wait_fences( *in_flight_fences_[frame_index_], VK_TRUE, UINT64_MAX );

      image_index_ = swapchain_->acquire_next_image( image_available_semaphores_[frame_index_] );
      hardware_->device().resetFences( *in_flight_fences_[frame_index_] );

      vk::CommandBufferBeginInfo command_buffer_begin_info{};
      command_buffers_[frame_index_].begin( command_buffer_begin_info );
    }
    catch ( vk::SystemError & e )
    {
      SPDLOG_LOGGER_ERROR( global::logger, "begin_frame, vk::SystemError: {} {}", e.what(), e.code().value() );
      return e.code().value();
    }
    catch ( ... )
    {
      SPDLOG_LOGGER_ERROR( global::logger, "begin_frame, unknown error" );
      return -1;
    }
    return 0;
  }

  int vk_backend::end_frame()
  {
    try
    {
      command_buffers_[frame_index_].end();

      {
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

        queue_cache_->queue( QueueType::GRAPHICS )->submit( submit_info, *in_flight_fences_[frame_index_] );
      }

      {
        vk::ArrayProxy<const vk::Semaphore>    presnet_wait_semaphores( *render_finished_semaphores_[frame_index_] );
        vk::ArrayProxy<const vk::SwapchainKHR> present_swapchains( *swapchain_->swaichain() );

        vk::PresentInfoKHR present_info{ .waitSemaphoreCount = presnet_wait_semaphores.size(),
                                         .pWaitSemaphores    = presnet_wait_semaphores.data(),
                                         .swapchainCount     = present_swapchains.size(),
                                         .pSwapchains        = present_swapchains.data(),
                                         .pImageIndices      = &image_index_ };

        auto result = queue_cache_->queue( QueueType::PRESENT )->presentKHR( present_info );
        if ( result == vk::Result::eSuboptimalKHR )
        {
        }
      }

      frame_index_ = ( frame_index_ + 1 ) % swapchain_->max_frames_in_flight();
    }
    catch ( vk::SystemError & e )
    {
      return e.code().value();
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

      render_pass_->begin( command_buffers_[frame_index_], framebuffers_[image_index_], render_area, clear_value );
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
