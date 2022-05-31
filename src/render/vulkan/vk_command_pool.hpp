#ifndef WABBY_RENDER_VULKAN_COMMAND_POOL
#define WABBY_RENDER_VULKAN_COMMAND_POOL

// comm include
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{

  class vk_command_pool : public vk::raii::CommandPool
  {
  public:
    vk_command_pool( const vk::raii::Device & device, uint32_t queue_family_index, vk::CommandPoolCreateFlags flags )
      : CommandPool( device, vk::CommandPoolCreateInfo{ .flags = flags, .queueFamilyIndex = queue_family_index } ){};
  };

}  // namespace wabby::render::vulkan

#endif