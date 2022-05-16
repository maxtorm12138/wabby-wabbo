#include "vk_backend.hpp"

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
    device_allocator_(environment_.instance(), hardware_.physical_device(), hardware_.device())
{}

}