#ifndef _WABBY_VULKAN_HARDWARE_HPP
#define _WABBY_VULKAN_HARDWARE_HPP

// module
#include "vk_defines.hpp"

// comm headers
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{
  class vk_hardware : public boost::noncopyable
  {
  public:
    vk_hardware( const vk::raii::Instance & instance, const vk::raii::SurfaceKHR & surface );

  public:
    const vk::raii::PhysicalDevice & physical_device() const
    {
      return physical_device_;
    }

    const vk::raii::Device & device() const
    {
      return device_;
    }

    std::optional<vk::raii::Queue> queue( QueueType type, const std::optional<std::reference_wrapper<const vk::raii::SurfaceKHR>> surface = {} ) const;

    std::optional<uint32_t> queue_index( QueueType type, const std::optional<std::reference_wrapper<const vk::raii::SurfaceKHR>> surface = {} ) const;

    std::vector<vk::raii::CommandBuffer> allocate_graphics_command_buffers( uint32_t size );

  private:
    std::shared_ptr<spdlog::logger>                 vulkan_logger_;
    vk::raii::PhysicalDevice                        physical_device_;
    vk::raii::Device                                device_;
    mutable std::unordered_map<QueueType, uint32_t> queue_index_cache_;
    vk::raii::CommandPool                           graphics_command_pool_;
  };
}  // namespace wabby::render::vulkan

#endif