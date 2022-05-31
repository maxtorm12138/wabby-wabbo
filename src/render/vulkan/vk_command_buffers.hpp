#ifndef WABBY_RENDER_VULKAN_COMMAND_BUFFERS
#define WABBY_RENDER_VULKAN_COMMAND_BUFFERS

// module
#include "vk_command_pool.hpp"

// comm headers
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{

  class vk_command_buffers : public vk_vector<vk::raii::CommandBuffer>
  {
  public:
    vk_command_buffers( const vk::raii::Device & device, const vk_command_pool & pool, uint32_t size, bool primary = true )
    {
      assert( size > 0 && "size must greater than zero" );
      reserve( size );

      vk::CommandBufferAllocateInfo allocate_info{ .commandPool        = *pool,
                                                   .level              = primary ? vk::CommandBufferLevel::ePrimary : vk::CommandBufferLevel::eSecondary,
                                                   .commandBufferCount = size };

      auto command_buffers = ( *device ).allocateCommandBuffers<vk_allocator<vk::CommandBuffer>>( allocate_info );
      for ( auto & buffer : command_buffers )
      {
        emplace_back( device, buffer, *pool );
      }
    }
  };

}  // namespace wabby::render::vulkan

#endif