#ifndef _WABBY_VULKAN_FRAMEBUFFERS_HPP
#define _WABBY_VULKAN_FRAMEBUFFERS_HPP

// comm includes
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{
  class vk_framebuffers : vk_vector<vk::raii::Framebuffer>
  {
  public:
    vk_framebuffers( const vk::raii::Device &                               device,
                     const vk::raii::RenderPass &                           render_pass,
                     size_t                                                 size,
                     const vk_vector<vk::ArrayProxy<const vk::ImageView>> & attachments,
                     vk::Extent2D                                           extent )
    {
      assert( size <= attachments.size() );
      reserve( size );
      for ( size_t i = 0; i < size; i++ )
      {
        vk::FramebufferCreateInfo frame_buffer_create_info{ .renderPass      = *render_pass,
                                                            .attachmentCount = attachments[i].size(),
                                                            .pAttachments    = attachments[i].data(),
                                                            .width           = extent.width,
                                                            .height          = extent.height,
                                                            .layers          = 1 };
        emplace_back( device, frame_buffer_create_info );
      }
    }
  };
}  // namespace wabby::render::vulkan

#endif