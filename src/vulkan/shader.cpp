#ifndef _WABBY_VULKAN_SHADER_HPP
#define _WABBY_VULKAN_SHADER_HPP

// module
#include "util/noncopyable.hpp"

// vulkan
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace wabby::vulkan
{

class shader : public util::noncopyable
{
public:
    shader();

public:
    vk::ShaderStageFlagBits stage() const { return stage_; };

    const std::string &name() const { return name_; }

    const vk::raii::ShaderModule &module() const { return module_; }

private:
    std::string name_;
    vk::ShaderStageFlagBits stage_;
    vk::raii::ShaderModule module_;
};

}

#endif