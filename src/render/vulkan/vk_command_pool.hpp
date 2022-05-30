#ifndef WABBY_RENDER_VULKAN_COMMAND_POOL
#define WABBY_RENDER_VULKAN_COMMAND_POOL

// comm include
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{

  class vk_command_pool : public boost::noncopyable
  {
  public:
    vk_command_pool( const vk::raii::Device & device, uint32_t queue_family_index, vk::CommandPoolCreateFlags flags )
      : pool_( device, vk::CommandPoolCreateInfo{ .flags = flags, .queueFamilyIndex = queue_family_index } ){

      };

  private:
    vk::raii::CommandPool pool_;
  };

}  // namespace wabby::render::vulkan

#endif