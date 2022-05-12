#ifndef _WAWY_VULKAN_HARDWARE_HPP
#define _WAWY_VULKAN_HARDWARE_HPP

// module
#include "util/noncopyable.hpp"
#include "util/api.hpp"
#include "defines.hpp"

// vulkan
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

// std
#include "optional"

namespace wabby::vulkan
{

class WAWY_API_HIDE hardware : public wabby::util::noncopyable
{
public:
    hardware(const vk::raii::Instance &instance, const vk::raii::SurfaceKHR &surface);    

public:
    const vk::raii::PhysicalDevice &physical_device() const { return physical_device_; }

    const vk::raii::Device &device() const { return device_; }

    std::optional<vk::raii::Queue> queue(QueueType type, const std::optional<std::reference_wrapper<vk::raii::SurfaceKHR>> surface = {});

    std::optional<uint32_t> queue_index(QueueType type, const std::optional<std::reference_wrapper<vk::raii::SurfaceKHR>> surface = {}) const;

private:
    vk::raii::PhysicalDevice physical_device_;
    vk::raii::Device device_;
    std::unordered_map<QueueType, uint32_t> queue_index_cache_;
};

}

#endif