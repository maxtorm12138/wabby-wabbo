#ifndef _WABBY_VULKAN_HARDWARE_HPP
#define _WABBY_VULKAN_HARDWARE_HPP

// module
#include "vk_defines.hpp"

// boost
#include "boost/noncopyable.hpp"

// vulkan
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

// std
#include "optional"
#include "unordered_map"

namespace wabby::render::vulkan
{
class vk_hardware : public boost::noncopyable
{
public:
    vk_hardware(const vk::raii::Instance &instance, const vk::raii::SurfaceKHR &surface);    

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