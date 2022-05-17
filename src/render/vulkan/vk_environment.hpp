#ifndef _WABBY_VULKAN_INSTANCE_HPP
#define _WABBY_VULKAN_INSTANCE_HPP

// comm headers
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{
class vk_environment : public boost::noncopyable
{
public:
    vk_environment(const vk::ApplicationInfo &application_info, const std::vector<std::string> &windowsystem_extensions);

public:
    const vk::raii::Context &context() const { return context_; };
    const vk::raii::Instance &instance() const { return instance_; }

private:
    std::vector<std::shared_ptr<spdlog::logger>> loggers_;
    vk::raii::Context context_;
    vk::raii::Instance instance_;
    vk::raii::DebugUtilsMessengerEXT debug_messenger_;
};
}
#endif