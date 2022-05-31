#include "wabby/core/engine.hpp"

#include "wabby/container/delayed.hpp"
#include "wabby/memory/memory.hpp"
#include "wabby/sdl2/sdl2.hpp"
#define WABBY_API_INCLUDE_IMPORT
#include "wabby/render/backend_raii.hpp"
#undef WABBY_API_INCLUDE_IMPORT

// boost
#include "boost/dll/runtime_symbol_info.hpp"
#include "boost/dll/shared_library.hpp"

// spdlog
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace wabby::core
{

  boost::dll::fs::path find_render_library_path()
  {
    auto path = boost::dll::program_location().parent_path();
#if BOOST_OS_WINDOWS
    return path / "wabbyvulkan.dll";
#else
    return path / "libwabbyvulkan.so";
#endif
  }

  class engine_impl : public boost::noncopyable
  {
  public:
    void setup( const engine_setup_info & setup_info );

    void run();

    void teardown();

  private:
    void handle_event( SDL_Event & event );

  private:
    container::delayed<spdlog::logger>        logger_;
    container::delayed<sdl2::context>         sdl_context_;
    container::delayed<sdl2::window>          window_;
    container::delayed<render::raii::backend> backend_;

    bool running_ = true;
    bool paused_  = false;
  };

  void engine_impl::setup( const engine_setup_info & setup_info )
  {
    logger_.construct( "engine", std::make_shared<spdlog::sinks::stderr_color_sink_mt>() );

    sdl_context_.construct();
    window_.construct( setup_info.application_name, setup_info.width, setup_info.height );

    auto library_path = find_render_library_path();

    backend_allocator_t allocator{ .user_args     = nullptr,
                                   .fn_allocation = []( void *, size_t size, size_t align ) { return memory::allocation( size, align ); },
                                   .fn_reallocation = []( void *, void * orig, size_t size, size_t align )
                                   { return memory::reallocation( orig, size, align ); },
                                   .fn_free = []( void *, void * mem ) { return memory::free( mem ); } };

    backend_.construct( library_path, &allocator );

    auto backend_configuration_path = ( boost::dll::program_location().parent_path() / "vulkan.ini" ).string();
    auto windowsystem_extensions    = window_->get_vulkan_instance_extensions();

    auto fn_vk_create_surface = []( void * that, VkInstance instance )
    { return static_cast<engine_impl *>( that )->window_->create_vulkan_surface( instance ); };

    auto fn_get_window_size = []( void * that, uint32_t * w, uint32_t * h )
    {
      auto size = static_cast<engine_impl *>( that )->window_->get_vulakn_drawable_size();
      *w        = size.first;
      *h        = size.second;
    };

    vk_backend_setup_info vk_backend_setup_info{
      .application_name    = setup_info.application_name,
      .application_version = setup_info.application_version,
      .configuration_path  = backend_configuration_path.c_str(),

      .windowsystem_extensions       = windowsystem_extensions.data(),
      .windowsystem_extensions_count = static_cast<uint32_t>( windowsystem_extensions.size() ),

      .user_args            = this,
      .fn_vk_create_surface = fn_vk_create_surface,
      .fn_get_window_size   = fn_get_window_size,
    };

    backend_->setup( &vk_backend_setup_info );
  }

  void engine_impl::run()
  {
    window_->set_resizeable( true );
    window_->show();
    running_ = true;
    paused_  = false;

    while ( running_ )
    {
      if ( paused_ )
      {
        auto event = sdl_context_->wait_event();
        handle_event( event );
      }
      else
      {
        while ( auto event = sdl_context_->poll_event() )
        {
          handle_event( *event );
        }
      }

      backend_->begin_frame();
      backend_->begin_render_pass();
      backend_->end_render_pass();
      backend_->end_frame();
    }
  }

  void engine_impl::teardown()
  {
    backend_->teardown();
  }

  void engine_impl::handle_event( SDL_Event & event )
  {
    if ( event.type == SDL_QUIT )
    {
      running_ = false;
    }
    else if ( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED )
    {
      SPDLOG_LOGGER_INFO( logger_, "window resized" );
      window_->set_resizeable( false );
      backend_->resized();
      window_->set_resizeable( true );
    }
    else if ( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_MINIMIZED )
    {
      SPDLOG_LOGGER_INFO( logger_, "window minimized" );
      paused_ = true;
    }
    else if ( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESTORED )
    {
      SPDLOG_LOGGER_INFO( logger_, "window restored" );
      paused_ = false;
    }
  }

  engine::engine() : impl_( new engine_impl ) {}

  engine::~engine()
  {
    delete impl_;
  }

  void engine::setup( const engine_setup_info & setup_info )
  {
    return impl_->setup( setup_info );
  }

  void engine::run()
  {
    return impl_->run();
  }

  void engine::teardown()
  {
    impl_->teardown();
  }

}  // namespace wabby::core