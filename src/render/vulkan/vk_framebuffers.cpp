#include "vk_framebuffers.hpp"

namespace wabby::render::vulkan
{

  std::vector<vk::raii::Framebuffer> build_framebuffers( const vk::raii::Device &                                 device,
                                                         const vk::raii::RenderPass &                             render_pass,
                                                         size_t                                                   size,
                                                         const std::vector<vk::ArrayProxy<const vk::ImageView>> & attachments,
                                                         vk::Extent2D                                             extent )
  {
    assert( size <= attachments.size() );
    std::vector<vk::raii::Framebuffer> framebuffers;
    framebuffers.reserve( size );

    for ( size_t i = 0; i < size; i++ )
    {
      vk::FramebufferCreateInfo frame_buffer_create_info{ .renderPass      = *render_pass,
                                                          .attachmentCount = attachments[i].size(),
                                                          .pAttachments    = attachments[i].data(),
                                                          .width           = extent.width,
                                                          .height          = extent.height,
                                                          .layers          = 1 };
      framebuffers.emplace_back( device, frame_buffer_create_info );
    }

    spdlog::get( "vulkan" )->info( "framebuffers create {}", size );
    return framebuffers;
  }

  vk_framebuffers::vk_framebuffers( const vk::raii::Device &                                 device,
                                    const vk::raii::RenderPass &                             render_pass,
                                    size_t                                                   size,
                                    const std::vector<vk::ArrayProxy<const vk::ImageView>> & attachments,
                                    vk::Extent2D                                             extent )
    : framebuffers_( build_framebuffers( device, render_pass, size, attachments, extent ) )
  {
  }

}  // namespace wabby::render::vulkan