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
    vk_hardware( const vk::raii::Instance & instance, const vk::SurfaceKHR & surface );

  public:
    const vk::raii::PhysicalDevice & physical_device() const;

    const vk::raii::Device & device() const;

  public:
    vk_vector<vk::raii::CommandBuffer> allocate_graphics_command_buffers( uint32_t size, bool primary = true );

  private:
    vk_vector<vk_string> check_extensions_supported_( const vk::PhysicalDevice & physical_device, const vk_vector<std::string_view> & desired_extensions );

    vk::raii::PhysicalDevice pick_physical_device_( const vk::raii::Instance & instance, const vk::SurfaceKHR & surface );

    vk::raii::Device build_device_();

  private:
    vk::raii::PhysicalDevice physical_device_;
    vk::raii::Device         device_;
  };

  class vk_queue_cache : public boost::noncopyable
  {
  public:
    enum class QueueType
    {
      PRESENT,
      GRAPHICS,
      COMPUTE,
      TRANSFER
    };

    vk_queue_cache( const vk_hardware & hardware, const vk::SurfaceKHR & surface );

  public:
    std::optional<vk::raii::Queue> queue( QueueType type );

    std::optional<uint32_t> queue_index( QueueType type );

  private:
    const vk_hardware &                   hardware_;
    const vk::SurfaceKHR &                surface_;
    vk_unordered_map<QueueType, uint32_t> cache_;
  };
}  // namespace wabby::render::vulkan

#endif