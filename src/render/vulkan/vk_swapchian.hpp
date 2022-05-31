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
    vk_swapchain( const vk_hardware &                           hardware,
                  const vk::raii::SurfaceKHR &                  surface,
                  vk_queue_cache &                              queue_cache,
                  std::function<void( uint32_t *, uint32_t * )> fn_get_window_size );

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

    const vk_vector<vk::raii::ImageView> & image_views() const
    {
      return image_views_;
    }

  public:
    void recreate( const vk_hardware & hardware, const vk::raii::SurfaceKHR & surface, vk_queue_cache & queue_cache );

    uint32_t acquire_next_image( const vk::raii::Semaphore & image_available_semaphore );

  private:
    vk::PresentModeKHR pick_present_mode( const vk::raii::PhysicalDevice & physical_device, const vk::raii::SurfaceKHR & surface );

    vk::SurfaceFormatKHR pick_surface_format( const vk::raii::PhysicalDevice & physical_device, const vk::raii::SurfaceKHR & surface );

    vk::Extent2D pick_extent( const vk::raii::PhysicalDevice & physical_device, const vk::raii::SurfaceKHR & surface );

    size_t get_image_count( const vk_hardware & hardware, const vk::raii::SurfaceKHR & surface );

    vk::raii::SwapchainKHR
      build_swapchian( const vk_hardware & hardware, const vk::raii::SurfaceKHR & surface, vk_queue_cache & queue_cache, bool recreate = false );

    vk_vector<vk::raii::ImageView> build_image_views( const vk_hardware & hardware );

  private:
    std::function<void( uint32_t *, uint32_t * )> fn_get_window_size_;
    vk::PresentModeKHR                            present_mode_;
    vk::SurfaceFormatKHR                          surface_format_;
    vk::Extent2D                                  extent_;
    size_t                                        image_count_;
    size_t                                        max_frames_in_flight_;
    vk::raii::SwapchainKHR                        swapchain_;
    vk_vector<vk::raii::ImageView>                image_views_;
  };

}  // namespace wabby::render::vulkan

#endif