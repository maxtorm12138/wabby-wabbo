#ifndef _WABBY_VULKAN_FRAMEBUFFERS_HPP
#define _WABBY_VULKAN_FRAMEBUFFERS_HPP

// comm includes
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{

  class vk_framebuffers : public boost::noncopyable
  {
  public:
    vk_framebuffers( const vk::raii::Device &                                 device,
                     const vk::raii::RenderPass &                             render_pass,
                     size_t                                                   size,
                     const std::vector<vk::ArrayProxy<const vk::ImageView>> & attachments,
                     vk::Extent2D                                             extent );

  public:
    const vk::raii::Framebuffer & framebuffer( size_t offset ) const
    {
      assert( offset < framebuffers_.size() );
      return framebuffers_[offset];
    }

  private:
    std::vector<vk::raii::Framebuffer> framebuffers_;
  };

}  // namespace wabby::render::vulkan

#endif