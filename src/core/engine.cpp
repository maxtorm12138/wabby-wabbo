#include "engine.hpp"

// sdl2
#include "sdl/sdl2.hpp"

// render
#include "render/backend.hpp"

// bosot
#include "boost/dll.hpp"

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
    sdl_context_(),
    window_(application_name, 800, 600),
    render_library_("wabbyvulkan", boost::dll::load_mode::append_decorations)
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