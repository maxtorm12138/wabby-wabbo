#ifndef WABBY_RENDER_VULKAN_SEMAPHORES
#define WABBY_RENDER_VULKAN_SEMAPHORES

// common include
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{
  class vk_semaphores : public vk_vector<vk::raii::Semaphore>
  {
  public:
    vk_semaphores( const vk::raii::Device & device, uint32_t size )
    {
      reserve( size );
      vk::SemaphoreCreateInfo semaphore_create_info{};
      for ( uint32_t i = 0; i < size; i++ )
      {
        emplace_back( device, semaphore_create_info );
      }
    }
  };
}  // namespace wabby::render::vulkan
#endif