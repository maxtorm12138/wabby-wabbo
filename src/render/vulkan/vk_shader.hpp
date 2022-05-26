#ifndef _WABBY_VULKAN_SHADER_HPP
#define _WABBY_VULKAN_SHADER_HPP

// comm headers
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{

  class vk_shader : public boost::noncopyable
  {
  public:
    vk_shader( vk_shader && other ) noexcept;

    vk_shader & operator=( vk_shader && other ) noexcept;

  public:
    vk::ShaderStageFlagBits stage() const
    {
      return stage_;
    };

    const std::string & name() const
    {
      return entry_;
    }

    const vk::raii::ShaderModule & module() const
    {
      return module_;
    }

    static vk_shader from_file( const vk::raii::Device & device, const std::filesystem::path & path, const std::string & entry, vk::ShaderStageFlagBits stage );

  private:
    vk_shader() = default;

  private:
    std::string             entry_{ "main" };
    vk::ShaderStageFlagBits stage_{};
    vk::raii::ShaderModule  module_{ nullptr };
  };

}  // namespace wabby::render::vulkan

#endif