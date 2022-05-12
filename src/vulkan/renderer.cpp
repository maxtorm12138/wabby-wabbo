#include "renderer.hpp"

// module
#include "miscellaneous.hpp"
#include "environment.hpp"
#include "hardware.hpp"

// vulkan
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace wabby::vulkan
{

vk::ApplicationInfo build_application_info(const renderer::create_info &create_info)
{
    return vk::ApplicationInfo
    {
        .pApplicationName = create_info.applicaiton_name.c_str(),
        .applicationVersion = create_info.application_version,
        .pEngineName = "wabby-vulkan",
        .engineVersion = WAWY_VULKAN_VERSION,
        .apiVersion = VK_API_VERSION_1_1
    };
}

class renderer_impl : public wabby::util::noncopyable
{
public:
    renderer_impl(const renderer::create_info &create_info);

public:
    void begin_frame() {}
    void end_frame() {}
    void begin_render_pass() {}
    void end_render_pass() {}

private:
    vulkan::environment environment_;
    vk::raii::SurfaceKHR surface_;
    vulkan::hardware hardware_;
};

renderer::renderer(const create_info &create_info) :
    impl_(new renderer_impl(create_info))
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

renderer_impl::renderer_impl(const renderer::create_info &create_info) :
    environment_(build_application_info(create_info), create_info.windowsystem_extensions),
    surface_(environment_.instance(), create_info.fn_make_surface(*environment_.instance())),
    hardware_(environment_.instance(), surface_)
{}

}