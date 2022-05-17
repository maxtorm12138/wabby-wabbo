#include "vk_swapchian.hpp"

// spdlog
#include "spdlog/spdlog.h"

namespace wabby::render::vulkan
{

vk::PresentModeKHR pick_present_mode(const vk::raii::PhysicalDevice &physical_device, const vk::raii::SurfaceKHR &surface)
{
    auto present_modes = physical_device.getSurfacePresentModesKHR(*surface);
    vk::PresentModeKHR mode = vk::PresentModeKHR::eFifo;
    for (const auto& m : present_modes)
    {
        if (m == vk::PresentModeKHR::eMailbox)
        {
            mode = m;
        }
    }
    spdlog::get("vulkan")->info("swapchain present mode: {}", vk::to_string(mode));
    return mode;
}

vk::SurfaceFormatKHR pick_surface_format(const vk::raii::PhysicalDevice &physical_device, const vk::raii::SurfaceKHR &surface)
{
    auto surface_formats = physical_device.getSurfaceFormatsKHR(*surface);
    auto surface_format = surface_formats[0];

    for (const auto& fmt: surface_formats)
    {
        if (fmt.format == vk::Format::eB8G8R8A8Srgb && fmt.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            surface_format = fmt;
        }
    }
    spdlog::get("vulkan")->info("swapchain surface format: {} {}", vk::to_string(surface_format.format), vk::to_string(surface_format.colorSpace));
    return surface_format;
}

vk::Extent2D pick_extent(const vk::raii::PhysicalDevice &physical_device, const vk::raii::SurfaceKHR &surface, std::pair<uint32_t, uint32_t> window_size)
{
    auto surface_capabilities = physical_device.getSurfaceCapabilitiesKHR(*surface);
    vk::Extent2D extent;
    if (surface_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        extent = surface_capabilities.currentExtent;
    }
    else
    {
        auto [w, h] = window_size;
        extent.width = std::clamp(static_cast<uint32_t>(w), surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
        extent.height = std::clamp(static_cast<uint32_t>(h), surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);
    }
    
    spdlog::get("vulkan")->info("swapchain extent: {} {}", extent.width, extent.height);
    return extent;
}

vk_swapchain::vk_swapchain(const vk::raii::PhysicalDevice &physical_device, const vk::raii::SurfaceKHR &surface, std::pair<uint32_t, uint32_t> window_size) :
    present_mode_(pick_present_mode(physical_device, surface)),
    surface_format_(pick_surface_format(physical_device, surface)),
    extent_(pick_extent(physical_device, surface, window_size)),
    swapchain_(nullptr),
    image_views_()
{}

}