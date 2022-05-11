#ifndef _WAWY_VULKAN_HARDWARE_HPP
#define _WAWY_VULKAN_HARDWARE_HPP

// module
#include "util/noncopyable.hpp"

// vulkan
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace wawy::vulkan
{

class hardware : public wawy::util::noncopyable
{
private:
    vk::raii::PhysicalDevice physical_device_;
};

}

#endif