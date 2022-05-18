#ifndef _WABBY_VULKAN_RENDER_PASS_HPP
#define _WABBY_VULKAN_RENDER_PASS_HPP

// comm headers
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{

  class vk_render_pass : public boost::noncopyable
  {
  public:
    vk_render_pass( const vk::raii::Device & device, const vk::SurfaceFormatKHR & surface_format );

  public:
    const vk::raii::RenderPass & render_pass() const
    {
      return render_pass_;
    }

  public:
    void begin( const vk::raii::CommandBuffer & buffer,
                const vk::raii::Framebuffer &   framebuffer,
                vk::Rect2D                      render_area,
                vk::ArrayProxy<vk::ClearValue>  clear_values );

    void end( const vk::raii::CommandBuffer & buffer );

  private:
    vk::raii::RenderPass render_pass_;
  };

}  // namespace wabby::render::vulkan
#endif