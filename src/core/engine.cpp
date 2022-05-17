#include "engine.hpp"

// sdl2
#include "sdl/sdl2.hpp"

// render
#include "render/backend.hpp"

// boost
#include "boost/dll/shared_library.hpp"
#include "boost/dll/runtime_symbol_info.hpp"

namespace wabby::core
{


class engine_impl : public boost::noncopyable
{
public:
    engine_impl(std::string_view application_name, uint32_t application_version);

public:
    void run();

private:
    sdl2::context sdl_context_;
    sdl2::window window_;
    boost::dll::shared_library render_library_;
    std::shared_ptr<render::backend> backend_;
};


engine::engine(std::string_view application_name, uint32_t application_version) :
    impl_(new engine_impl(application_name, application_version))
{}

engine::~engine()
{
    delete impl_;
}

void engine::run()
{
    return impl_->run();
}

boost::dll::fs::path find_render_library() {
    auto path = boost::dll::program_location().parent_path();
#if BOOST_OS_WINDOWS
    return path / "wabbyvulkan.dll";
#else
    return path / "libwabbyvulkan.so";
#endif
}

engine_impl::engine_impl(std::string_view application_name, uint32_t application_version) :
    sdl_context_(),
    window_(application_name, 800, 600),
    render_library_(find_render_library()),
    backend_(render_library_.get_alias<decltype(wabby::render::make_vk_backend)>("make_vk_backend")(render::vk_backend_create_info{
        .applicaiton_name = application_name.data(),
        .application_version = application_version,
        .windowsystem_extensions = window_.get_vulkan_instance_extensions(),
        .fn_make_surface = [this](VkInstance instance) { return window_.create_vulkan_surface(instance); },
        .fn_get_window_size = [this](){ return window_.get_vulakn_drawable_size(); }
    }))
{
}

void engine_impl::run()
{
    window_.show();
    bool running = true;
    while (running)
    {
        while (auto event = sdl_context_.poll_event())
        {
            if (event->type == SDL_QUIT)
            {
                running = false;
            }
        }
    }
}
}