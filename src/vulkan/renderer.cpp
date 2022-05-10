#include "renderer.hpp"

// module
#include "miscellaneous.hpp"
#include "instance.hpp"

// vulkan
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace wawy::vulkan
{

class renderer_impl : public wawy::util::noncopyable
{
public:
    renderer_impl(const wawy::sdl2::window &window);

public:
    void begin_frame();
    void end_frame();
    void begin_render_pass();
    void end_render_pass();

private:
    vk::raii::Context context_;
    vk::raii::Instance instance_;
    vk::raii::DebugUtilsMessengerEXT debug_messenger_;
};


renderer::renderer(const wawy::sdl2::window &window) :
    impl_(new renderer_impl(window))
{}

renderer::~renderer()
{
    delete impl_;
}

void renderer::begin_frame()
{
    return impl_->begin_frame();
}
void renderer::end_frame()
{
    return impl_->end_frame();
}

void renderer::begin_render_pass()
{
    return impl_->begin_render_pass();
}

void renderer::end_render_pass()
{
    return impl_->end_render_pass();
}

renderer_impl::renderer_impl(const wawy::sdl2::window &window) :
    context_(),
    instance_(wawy::vulkan::instance::build(context_, {}, window.get_vulkan_instance_extensions())),
#ifdef NDEBUG
    debug_messenger_(nullptr),
#else
    debug_messenger_(instance_, misc::DEBUG_MESSENGER_CREATE_INFO)
#endif
{}

}