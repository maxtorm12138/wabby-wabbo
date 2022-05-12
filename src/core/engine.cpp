#include "engine.hpp"

#define WAWY_API_IMPORT
#include "vulkan/renderer.hpp"
#include "sdl2/sdl2.hpp"
#undef WAWY_API_IMPORT

namespace wabby::core
{


class engine_impl : public wabby::util::noncopyable
{
public:
    engine_impl(std::string_view application_name, uint32_t application_version);

public:
    void run();

private:
    sdl2::context sdl_context_;
    sdl2::window window_;
    vulkan::renderer renderer_;
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


engine_impl::engine_impl(std::string_view application_name, uint32_t application_version) :
    window_(application_name, 800, 600),
    renderer_(vulkan::renderer::create_info {
        .applicaiton_name = application_name.data(),
        .application_version = application_version,
        .windowsystem_extensions = window_.get_vulkan_instance_extensions(),
        .fn_make_surface = [this](VkInstance instance) { return window_.create_vulkan_surface(instance); },
        .fn_get_window_size = [this]() { return window_.get_vulakn_drawable_size();},
    })
{}

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