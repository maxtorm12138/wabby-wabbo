#ifndef _WAWY_VULKAN_INSTANCE_HPP
#define _WAWY_VULKAN_INSTANCE_HPP

// module
#include "util/noncopyable.hpp"

// vulkan
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace wawy::vulkan::instance
{
vk::raii::Instance build(
    const vk::raii::Context &context,
    const vk::ApplicationInfo &application_info,
    const std::vector<std::string> &desired_extensions);
}
#endif