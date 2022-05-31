#ifndef _WABBY_VULKAN_FRAMEBUFFERS_HPP
#define _WABBY_VULKAN_FRAMEBUFFERS_HPP

#include "vk_swapchian.hpp"

// comm includes
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{
  class vk_framebuffers : public vk_vector<vk::raii::Framebuffer>
  {
  public:
    vk_framebuffers( const vk::raii::Device & device, const vk::raii::RenderPass & render_pass, const vk_swapchain & swapchain )
    {
      reserve( swapchain.image_count() );

      for ( size_t i = 0; i < swapchain.image_count(); i++ )
      {
        vk::ArrayProxy<const vk::ImageView> attachments( *swapchain.image_views()[i] );
        vk::FramebufferCreateInfo           frame_buffer_create_info{ .renderPass      = *render_pass,
                                                                      .attachmentCount = attachments.size(),
                                                                      .pAttachments    = attachments.data(),
                                                                      .width           = swapchain.extent().width,
                                                                      .height          = swapchain.extent().height,
                                                                      .layers          = 1 };
        emplace_back( device, frame_buffer_create_info );
      }
    }

    void recreate( const vk::raii::Device & device, const vk::raii::RenderPass & render_pass, const vk_swapchain & swapchain )
    {
      clear();
      reserve( swapchain.image_count() );

      for ( size_t i = 0; i < swapchain.image_count(); i++ )
      {
        vk::ArrayProxy<const vk::ImageView> attachments( *swapchain.image_views()[i] );
        vk::FramebufferCreateInfo           frame_buffer_create_info{ .renderPass      = *render_pass,
                                                                      .attachmentCount = attachments.size(),
                                                                      .pAttachments    = attachments.data(),
                                                                      .width           = swapchain.extent().width,
                                                                      .height          = swapchain.extent().height,
                                                                      .layers          = 1 };
        emplace_back( device, frame_buffer_create_info );
      }
    }
  };
}  // namespace wabby::render::vulkan

#endif