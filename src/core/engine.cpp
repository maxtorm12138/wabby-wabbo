#include "engine.hpp"

// container
#include "container/registry.hpp"

// sdl2
#include "sdl/sdl2.hpp"

// render
#include "render/backend.hpp"

// boost
#include "boost/dll/runtime_symbol_info.hpp"
#include "boost/dll/shared_library.hpp"

// spdlog
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace wabby::core
{

  class engine_impl : public boost::noncopyable
  {
  public:
    engine_impl( const std::string & application_name, uint32_t application_version );

  public:
    void run();

  private:
    std::string                application_name_;
    uint32_t                   application_version_;
    sdl2::context              sdl_context_;
    sdl2::window               window_;
    boost::dll::shared_library render_library_;
  };

  boost::dll::fs::path find_render_library()
  {
    auto path = boost::dll::program_location().parent_path();
#if BOOST_OS_WINDOWS
    return path / "wabbyvulkan.dll";
#else
    return path / "libwabbyvulkan.so";
#endif
  }

  engine_impl::engine_impl( const std::string & application_name, uint32_t application_version )
    : application_name_( application_name )
    , application_version_( application_version )
    , sdl_context_()
    , window_( application_name, 800, 600 )
    , render_library_( find_render_library() )
  {
  }

  void engine_impl::run()
  {
    auto make_vk_backend = render_library_.get_alias<decltype( wabby::render::make_vk_backend )>( "make_vk_backend" );
    auto backend         = make_vk_backend();

    auto fn_make_surface    = [this]( VkInstance instance ) { return window_.create_vulkan_surface( instance ); };
    auto fn_get_window_size = [this]() { return window_.get_vulakn_drawable_size(); };

    render::vk_backend_create_info vk_backend_create_info;
    vk_backend_create_info.applicaiton_name        = application_name_;
    vk_backend_create_info.application_version     = application_version_;
    vk_backend_create_info.windowsystem_extensions = window_.get_vulkan_instance_extensions();
    vk_backend_create_info.fn_make_surface         = fn_make_surface;
    vk_backend_create_info.fn_get_window_size      = fn_get_window_size;

    backend->setup( vk_backend_create_info );

    window_.show();
    bool running = true;
    while ( running )
    {
      while ( auto event = sdl_context_.poll_event() )
      {
        switch ( event->type )
        {
          case SDL_QUIT: break;
          default: break;
        }

        if ( event->type == SDL_QUIT )
        {
          running = false;
        }
      }

      backend->begin_frame();
      backend->begin_render_pass();
      backend->end_render_pass();
      backend->end_frame();
    }

    backend->teardown();
  }

  engine::engine( std::string application_name, uint32_t application_version )
    : application_name_( application_name ), application_version_( application_version )
  {
  }

  engine::~engine() {}

  void engine::setup()
  {
    container::registry::instance().sign_in( "engine", spdlog::stderr_color_mt( "engine" ) );
    container::registry::instance().sign_in( "vulkan", spdlog::stderr_color_mt( "vulkan" ) );
    container::registry::instance().sign_in( "vulkan-debugcallback", spdlog::stderr_color_mt( "vulkan-debugcallback" ) );
    impl_ = new engine_impl( application_name_, application_version_ );
  }

  void engine::run()
  {
    return impl_->run();
  }

  void engine::teardown()
  {
    delete impl_;
  }

}  // namespace wabby::core