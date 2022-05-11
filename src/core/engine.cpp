#include "engine.hpp"

#define WAWY_API_IMPORT
#include "vulkan/renderer.hpp"
#include "sdl2/sdl2.hpp"
#undef WAWY_API_IMPORT

namespace wawy::core
{

class engine_impl : public wawy::util::noncopyable
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
    renderer_(window_, application_name, application_version)
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