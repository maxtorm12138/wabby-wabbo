#ifndef _WAWY_VULKAN_HARDWARE_HPP
#define _WAWY_VULKAN_HARDWARE_HPP

// module
#include "util/noncopyable.hpp"
#include "util/api.hpp"

// vulkan
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace wawy::vulkan
{

class WAWY_API_HIDE hardware : public wawy::util::noncopyable
{
public:
    hardware(const vk::raii::Instance &instance, const vk::raii::SurfaceKHR &surface);    

public:
    const vk::raii::PhysicalDevice &physical_device() const { return physical_device_; }
    const vk::raii::Device &device() const { return device_; }

private:
    vk::raii::PhysicalDevice physical_device_;
    vk::raii::Device device_;
};

}

#endif