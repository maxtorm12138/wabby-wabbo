#ifndef _WABBY_VULKAN_INSTANCE_HPP
#define _WABBY_VULKAN_INSTANCE_HPP

// module
#include "util/noncopyable.hpp"
#include "util/api.hpp"

// vulkan
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace wabby::vulkan
{
class WABBY_API_HIDE environment : public wabby::util::noncopyable
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
}
#endif