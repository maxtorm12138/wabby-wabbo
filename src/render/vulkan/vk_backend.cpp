#include "vk_backend.hpp"

namespace wabby::render
{
std::shared_ptr<backend> make_vk_backend(const vk_backend_create_info &create_info)
{
    return std::shared_ptr<backend>(new ::wabby::render::vulkan::vk_backend(create_info));
}
}

BOOST_DLL_ALIAS(wabby::render::make_vk_backend, make_vk_backend);

namespace wabby::render::vulkan
{

vk::ApplicationInfo build_application_info(const vk_backend_create_info &create_info)
{
    return vk::ApplicationInfo
    {
        .pApplicationName = create_info.applicaiton_name.c_str(),
        .applicationVersion = create_info.application_version,
        .pEngineName = WABBY_ENGINE_NAME.data(),
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_1
    };
}

vk_backend::vk_backend(const vk_backend_create_info &create_info) :
    environment_(build_application_info(create_info), create_info.windowsystem_extensions),
    surface_(environment_.instance(), create_info.fn_make_surface(*environment_.instance())),
    hardware_(environment_.instance(), surface_),
    device_allocator_(environment_.instance(), hardware_.physical_device(), hardware_.device()),
    swapchain_(hardware_, surface_, create_info.fn_get_window_size()),
    render_pass_(hardware_.device(), swapchain_.surface_format())
{}


void vk_backend::begin_frame()
{}

void vk_backend::end_frame()
{}

void vk_backend::begin_render_pass()
{}

void vk_backend::end_render_pass()
{}

void vk_backend::resized()
{}

}