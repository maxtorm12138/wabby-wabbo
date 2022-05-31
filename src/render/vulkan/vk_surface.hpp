#ifndef WABBY_RENDER_VULKAN_SURFACE_HPP
#define WABBY_RENDER_VULKAN_SURFACE_HPP
// common include
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{

  class vk_surface : public vk::raii::SurfaceKHR
  {
  public:
    vk_surface( const vk::raii::Instance & instance, std::function<VkSurfaceKHR( VkInstance instance )> fn_vk_create_surface )
      : SurfaceKHR( instance, fn_vk_create_surface( *instance ) )
    {
    }
  };
}  // namespace wabby::render::vulkan

#endif