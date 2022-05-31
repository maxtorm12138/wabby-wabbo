#ifndef WABBY_RENDER_VULKAN_FENCES
#define WABBY_RENDER_VULKAN_FENCES

// common include
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{
  class vk_fences : public vk_vector<vk::raii::Fence>
  {
  public:
    vk_fences( const vk::raii::Device & device, uint32_t size, bool signaled = true )
    {
      reserve( size );

      vk::FenceCreateFlags fence_create_flags;
      if ( signaled )
      {
        fence_create_flags |= vk::FenceCreateFlagBits::eSignaled;
      }

      vk::FenceCreateInfo fence_create_info{ .flags = fence_create_flags };
      for ( uint32_t i = 0; i < size; i++ )
      {
        emplace_back( device, fence_create_info );
      }
    }
  };
}  // namespace wabby::render::vulkan

#endif