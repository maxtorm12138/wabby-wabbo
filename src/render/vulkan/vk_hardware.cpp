#include "vk_hardware.hpp"

// std
#include "unordered_set"

// spdlog
#include "spdlog/spdlog.h"

namespace wabby::render::vulkan
{

const std::vector<std::string_view> REQUIRED_DEVICE_EXTENSION { EXT_NAME_VK_KHR_swapchain };
const std::vector<std::string_view> OPTIONAL_DEVICE_EXTENSION { EXT_NAME_VK_KHR_portability_subset };

vk::raii::PhysicalDevice pick_physical_device(const vk::raii::Instance &instance, const vk::raii::SurfaceKHR &surface);

std::vector<std::string> check_extensions_supported(const vk::raii::PhysicalDevice &physical_device, const std::vector<std::string_view> &desired_extensions);

vk::raii::Device build_device(const vk::raii::PhysicalDevice &physical_device);

std::optional<uint32_t> get_present_queue_index(const vk::raii::PhysicalDevice &physical_device, const vk::raii::SurfaceKHR &surface);

std::optional<uint32_t> get_first_queue_index(const vk::raii::PhysicalDevice &physical_device, vk::QueueFlags type);

vk_hardware::vk_hardware(const vk::raii::Instance &instance, const vk::raii::SurfaceKHR &surface) :
    physical_device_(pick_physical_device(instance, surface)),
    device_(build_device(physical_device_))
{}


std::optional<vk::raii::Queue> vk_hardware::queue(QueueType type, const std::optional<std::reference_wrapper<const vk::raii::SurfaceKHR>> surface) const
{
    if (auto cache = queue_index_cache_.find(type); cache != queue_index_cache_.end())
    {
        return device_.getQueue(cache->second, 0);
    }

    auto index = queue_index(type, surface);
    if (!index.has_value())
    {
        return {};
    }

    return device_.getQueue(*index, 0);
}

std::optional<uint32_t> vk_hardware::queue_index(QueueType type, const std::optional<std::reference_wrapper<const vk::raii::SurfaceKHR>> surface) const
{
    if (auto cache = queue_index_cache_.find(type); cache != queue_index_cache_.end())
    {
        return cache->second;
    }

    std::optional<uint32_t> index;
    switch (type) 
    {
        case QueueType::PRESENT:
            index = get_present_queue_index(physical_device_, surface->get());
            break;
        case QueueType::GRAPHICS:
            index = get_first_queue_index(physical_device_, vk::QueueFlagBits::eGraphics);
            break;
        case QueueType::COMPUTE:
            throw std::runtime_error("unsupported now");
            break;
        case QueueType::TRANSFER:
            throw std::runtime_error("unsupported now");
            break;
    }

    if (index.has_value())
    {
        queue_index_cache_[type] = *index;
    }

    spdlog::get("vulkan")->info("hardware queue_index {} {}", vk::to_string(type), index.has_value() ? *index : UINT32_MAX);
    return index;
}


std::optional<uint32_t> get_present_queue_index(const vk::raii::PhysicalDevice &physical_device, const vk::raii::SurfaceKHR &surface)
{
    auto queue_family_properties = physical_device.getQueueFamilyProperties();
    for (uint32_t i = 0; i < queue_family_properties.size(); i++)
    {
        if (physical_device.getSurfaceSupportKHR(i, *surface))
        {
            return i;
        }
    }

    return {};
}

std::optional<uint32_t> get_first_queue_index(const vk::raii::PhysicalDevice &physical_device, vk::QueueFlags type)
{
    auto queue_family_properties = physical_device.getQueueFamilyProperties();
    for (uint32_t i = 0; i < queue_family_properties.size(); i++)
    {
        if (queue_family_properties[i].queueFlags & type)
        {
            return i;
        }
    }

    return {};
}

vk::raii::PhysicalDevice pick_physical_device(const vk::raii::Instance &instance, const vk::raii::SurfaceKHR &surface)
{
    for (auto &physical_device : instance.enumeratePhysicalDevices())
    {
        auto properties = physical_device.getProperties();
        auto features = physical_device.getFeatures();

        if (properties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu)
        {
            continue;
        }

        if (!features.tessellationShader)
        {
            continue;
        }

        if (properties.apiVersion < VK_API_VERSION_1_1)
        {
            continue;
        }

        auto required_extensions = check_extensions_supported(physical_device, REQUIRED_DEVICE_EXTENSION);
        if (required_extensions.size() != REQUIRED_DEVICE_EXTENSION.size())
        {
            continue;
        }

        auto present_modes = physical_device.getSurfacePresentModesKHR(*surface);
        if (present_modes.empty())
        {
            continue;
        }

        auto surface_formats = physical_device.getSurfaceFormatsKHR(*surface);
        if (surface_formats.empty())
        {
            continue;
        }

        auto present_queue_index = get_present_queue_index(physical_device, surface);
        if (!present_queue_index.has_value())
        {
            continue;
        }

        auto graphics_queue_index = get_first_queue_index(physical_device, vk::QueueFlagBits::eGraphics);
        if (!graphics_queue_index.has_value())
        {
            continue;
        }

        spdlog::get("vulkan")->info("hardware physical device: {}", properties.deviceName);
        return std::move(physical_device);
    }

    throw std::runtime_error("no suitable gpu found");
}

std::vector<std::string> check_extensions_supported(const vk::raii::PhysicalDevice &physical_device, const std::vector<std::string_view> &desired_extensions)
{
    auto extension_props = physical_device.enumerateDeviceExtensionProperties();
    std::unordered_set<std::string_view> extensions;
    std::transform(extension_props.begin(), extension_props.end(), std::inserter(extensions, extensions.end()), [](auto &&prop) { return prop.extensionName.data(); });

    std::vector<std::string> result;
    result.reserve(desired_extensions.size());
    for (auto extension : desired_extensions) 
    {
        if (extensions.contains(extension))
        {
            result.push_back(extension.data());
        }
    }
    return result;
}

vk::raii::Device build_device(const vk::raii::PhysicalDevice &physical_device)
{
    auto required_extensions = check_extensions_supported(physical_device, REQUIRED_DEVICE_EXTENSION);
    auto optional_extensions = check_extensions_supported(physical_device, OPTIONAL_DEVICE_EXTENSION);
    
    std::vector<const char *> enable_extensions;
    std::transform(required_extensions.begin(), required_extensions.end(), std::back_inserter(enable_extensions), [](auto &&ext){ return ext.c_str(); });
    std::transform(optional_extensions.begin(), optional_extensions.end(), std::back_inserter(enable_extensions), [](auto &&ext){ return ext.c_str(); });

    auto queue_families = physical_device.getQueueFamilyProperties();
    std::vector<vk::DeviceQueueCreateInfo> device_queue_create_infos;
    std::vector<float> device_queue_priorities(queue_families.size(), 1.f);
    for (uint32_t i = 0; i < queue_families.size(); i++) 
    {
        device_queue_create_infos.push_back(vk::DeviceQueueCreateInfo{.queueFamilyIndex = i,.queueCount = 1,.pQueuePriorities = &device_queue_priorities[i]});
    }

    vk::PhysicalDeviceFeatures physical_device_features{};

    vk::DeviceCreateInfo device_create_info
    {
        .queueCreateInfoCount = static_cast<uint32_t>(device_queue_create_infos.size()),
        .pQueueCreateInfos = device_queue_create_infos.data(),
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<uint32_t>(enable_extensions.size()),
        .ppEnabledExtensionNames = enable_extensions.data(),
        .pEnabledFeatures = &physical_device_features
    };

    return vk::raii::Device(physical_device, device_create_info);
}

}