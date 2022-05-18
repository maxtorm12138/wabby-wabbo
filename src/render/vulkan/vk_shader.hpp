#ifndef _WABBY_VULKAN_SHADER_HPP
#define _WABBY_VULKAN_SHADER_HPP

// comm headers
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{

  class vk_shader : public boost::noncopyable
  {
  public:
    vk_shader( const std::string & name, vk::ShaderStageFlagBits stage );

  public:
    vk::ShaderStageFlagBits stage() const
    {
      return stage_;
    };

    const std::string & name() const
    {
      return name_;
    }

    const vk::raii::ShaderModule & module() const
    {
      return module_;
    }

  private:
    std::string             name_;
    vk::ShaderStageFlagBits stage_;
    vk::raii::ShaderModule  module_;
  };

}  // namespace wabby::render::vulkan

#endif