#include "engine.hpp"

// container
#include "wabby/container/delayed.hpp"

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
    sdl2::context                    sdl_context_;
    container::delayed<sdl2::window> window_;
  };

  void engine_impl::setup( const engine_setup_info & setup_info )
  {
    registry_ = std::make_shared<container::registry>();

    {
      spdlog::logger vulkan( "vulkan", std::make_shared<spdlog::sinks::stdout_color_sink_mt>() );
      vulkan.set_level( spdlog::level::debug );

      spdlog::logger vulkan_debugcallback( "vulkan-debugcallback", std::make_shared<spdlog::sinks::stdout_color_sink_mt>() );
      vulkan_debugcallback.set_level( spdlog::level::debug );

      registry_->sign_in( vulkan.name(), vulkan );
      registry_->sign_in( vulkan_debugcallback.name(), vulkan_debugcallback );

      registry_->get<spdlog::logger>( "vulkan" )->info( "" );
    }

    window_         = std::make_unique<sdl2::window>( setup_info.application_name, setup_info.width, setup_info.height );
    render_library_ = std::make_unique<boost::dll::shared_library>( find_render_library_path() );

    auto make_vk_backend = render_library_->get_alias<decltype( wabby::render::make_vk_backend )>( "make_vk_backend" );
    backend_             = make_vk_backend();

    auto fn_make_surface    = [this]( VkInstance instance ) { return window_->create_vulkan_surface( instance ); };
    auto fn_get_window_size = [this]() { return window_->get_vulakn_drawable_size(); };

    render::vk_backend_create_info vk_backend_create_info;
    vk_backend_create_info.applicaiton_name        = setup_info.application_name;
    vk_backend_create_info.application_version     = setup_info.application_version;
    vk_backend_create_info.registry                = registry_;
    vk_backend_create_info.windowsystem_extensions = window_->get_vulkan_instance_extensions();
    vk_backend_create_info.fn_make_surface         = fn_make_surface;
    vk_backend_create_info.fn_get_window_size      = fn_get_window_size;

    backend_->setup( vk_backend_create_info );
  }

  void engine_impl::run()
  {
    window_->show();
    bool running = true;
    while ( running )
    {
      while ( auto event = sdl_context_.poll_event() )
      {
        if ( event->type == SDL_QUIT )
        {
          running = false;
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