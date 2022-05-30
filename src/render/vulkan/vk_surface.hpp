#ifndef WABBY_RENDER_VULKAN_SURFACE_HPP
#define WABBY_RENDER_VULKAN_SURFACE_HPP
// common include
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{

  class vk_surface : public vk::raii::SurfaceKHR
  {
  public:
    vk_surface( const vk::raii::Instance & instance, pfn_vk_create_surface fn_create_surface, void * user_args )
      : SurfaceKHR( instance, fn_create_surface( user_args, *instance ) )
    {
    }
  };
}  // namespace wabby::render::vulkan

#endif