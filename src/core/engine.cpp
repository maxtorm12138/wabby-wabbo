#include "wabby/core/engine.hpp"

#include "wabby/container/delayed.hpp"
#include "wabby/render/backend_raii.hpp"
#include "wabby/sdl2/sdl2.hpp"

// boost
#include "boost/align/aligned_alloc.hpp"
#include "boost/dll/runtime_symbol_info.hpp"
#include "boost/dll/shared_library.hpp"

// spdlog
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace wabby::core
{

  void * fn_allocation( void * user_args, size_t size, size_t alignment )
  {
    return boost::alignment::aligned_alloc( alignment, size );
  }

  void * fn_reallocation( void * user_args, void * original, size_t size, size_t alignment )
  {
    std::abort();
    return nullptr;
  }

  void fn_free( void * user_args, void * memory )
  {
    boost::alignment::aligned_free( memory );
  }

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
    container::delayed<sdl2::context>         sdl_context_;
    container::delayed<sdl2::window>          window_;
    container::delayed<render::raii::backend> backend_;
  };

  void engine_impl::setup( const engine_setup_info & setup_info )
  {
    sdl_context_.construct();
    window_.construct( setup_info.application_name, setup_info.width, setup_info.height );

    auto library_path = find_render_library_path();

    allocation_callbacks allocation_callbacks{ .user_args = nullptr, .allocation = &fn_allocation, .reallocation = &fn_reallocation, .free = &fn_free };

    backend_.construct( library_path, &allocation_callbacks );

    auto backend_configuration_path = ( boost::dll::program_location().parent_path() / "vulkan.ini" ).string();
    auto windowsystem_extensions    = window_->get_vulkan_instance_extensions();

    auto fn_make_surface = []( void * that, VkInstance instance ) { return static_cast<engine_impl *>( that )->window_->create_vulkan_surface( instance ); };

    auto fn_get_window_size = []( void * that, uint32_t * w, uint32_t * h )
    {
      auto size = static_cast<engine_impl *>( that )->window_->get_vulakn_drawable_size();
      *w        = size.first;
      *h        = size.second;
    };

    vk_backend_setup_info vk_backend_setup_info{
      .application_name    = setup_info.application_name,
      .application_version = setup_info.application_version,

      .windowsystem_extensions       = windowsystem_extensions.data(),
      .windowsystem_extensions_count = static_cast<uint32_t>( windowsystem_extensions.size() ),

      .fn_make_surface_user_args = this,
      .fn_make_surface           = fn_make_surface,

      .fn_get_window_size_user_args = this,
      .fn_get_window_size           = fn_get_window_size,

      .configuration_path = backend_configuration_path.c_str(),
    };

    backend_->setup( reinterpret_cast<const backend_setup_info *>( &vk_backend_setup_info ) );
  }

  void engine_impl::run()
  {
    window_->set_resizeable( true );
    window_->show();
    bool running = true;
    while ( running )
    {
      while ( auto event = sdl_context_->poll_event() )
      {
        if ( event->type == SDL_QUIT )
        {
          running = false;
        }
        else if ( event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED )
        {
          window_->set_resizeable( false );
          window_->set_resizeable( true );
        }
        else if ( event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_MINIMIZED )
        {
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