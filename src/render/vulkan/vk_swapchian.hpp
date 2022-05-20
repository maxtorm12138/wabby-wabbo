#ifndef _WABBY_VULKAN_SWAPCHAIN_HPP
#define _WABBY_VULKAN_SWAPCHAIN_HPP

// moduel
#include "vk_hardware.hpp"

// comm headers
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{

  class vk_swapchain : public boost::noncopyable
  {
  public:
    vk_swapchain( const vk_hardware & hardware, const vk::raii::SurfaceKHR & surface, std::pair<uint32_t, uint32_t> window_size );

  public:
    vk::PresentModeKHR present_mode() const
    {
      return present_mode_;
    }

    vk::SurfaceFormatKHR surface_format() const
    {
      return surface_format_;
    }

    vk::Extent2D extent() const
    {
      return extent_;
    }

    size_t image_count() const
    {
      return image_count_;
    }

    size_t max_frames_in_flight() const
    {
      return max_frames_in_flight_;
    }

    const vk::raii::SwapchainKHR & swaichain() const
    {
      return swapchain_;
    }

    const std::vector<vk::raii::ImageView> & image_views() const
    {
      return image_views_;
    }

  public:
    uint32_t acquire_next_image( const vk::raii::Semaphore & image_available_semaphore );

  private:
    vk::PresentModeKHR               present_mode_;
    vk::SurfaceFormatKHR             surface_format_;
    vk::Extent2D                     extent_;
    size_t                           image_count_;
    size_t                           max_frames_in_flight_;
    vk::raii::SwapchainKHR           swapchain_;
    std::vector<vk::raii::ImageView> image_views_;
  };

}  // namespace wabby::render::vulkan

#endif