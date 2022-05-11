#ifndef _WAWY_VULKAN_INSTANCE_HPP
#define _WAWY_VULKAN_INSTANCE_HPP

// module
#include "util/noncopyable.hpp"

// vulkan
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace wawy::vulkan
{

class environment : public wawy::util::noncopyable
{
public:
    environment(const vk::ApplicationInfo &application_info, const std::vector<std::string> &windowsystem_extensions);

public:
    const vk::raii::Context &context() const { return context_; };
    const vk::raii::Instance &instance() const { return instance_; }

private:
    vk::raii::Context context_;
    vk::raii::Instance instance_;
    vk::raii::DebugUtilsMessengerEXT debug_messenger_;
};

vk::raii::Instance build(
    const vk::raii::Context &context,
    const vk::ApplicationInfo &application_info,
    const std::vector<std::string> &desired_extensions);
}
#endif