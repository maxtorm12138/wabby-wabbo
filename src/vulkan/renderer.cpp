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
    renderer_impl(const wawy::sdl2::window &window, std::string_view application_name, uint32_t application_version);

public:
    void begin_frame() {}
    void end_frame() {}
    void begin_render_pass() {}
    void end_render_pass() {}

private:
    vk::raii::Context context_;
    vk::raii::Instance instance_;
    vk::raii::DebugUtilsMessengerEXT debug_messenger_;
};

renderer::renderer(const wawy::sdl2::window &window, std::string_view application_name, uint32_t application_version) :
    impl_(new renderer_impl(window, application_name, application_version))
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

renderer_impl::renderer_impl(const wawy::sdl2::window &window, std::string_view application_name, uint32_t application_version) :
    context_(),
    instance_(wawy::vulkan::instance::build(
        context_,
        { .pApplicationName = application_name.data(), .applicationVersion = application_version, .pEngineName = "wabby-waboo vulkan", .engineVersion = WAWY_VULKAN_VERSION, .apiVersion = VK_API_VERSION_1_1, },
        window.get_vulkan_instance_extensions())),
#ifdef NDEBUG
    debug_messenger_(nullptr),
#else
    debug_messenger_(instance_, misc::DEBUG_MESSENGER_CREATE_INFO)
#endif
{}

}