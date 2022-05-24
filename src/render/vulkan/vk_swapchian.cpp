#include "vk_swapchian.hpp"

// spdlog
#include "spdlog/spdlog.h"

// std
#include "unordered_set"

namespace wabby::render::vulkan
{

  vk::PresentModeKHR pick_present_mode( const vk::raii::PhysicalDevice & physical_device, const vk::raii::SurfaceKHR & surface )
  {
    auto               present_modes = physical_device.getSurfacePresentModesKHR( *surface );
    vk::PresentModeKHR mode          = vk::PresentModeKHR::eFifo;
    for ( const auto & m : present_modes )
    {
      if ( m == vk::PresentModeKHR::eMailbox )
      {
        mode = m;
      }
    }
    return mode;
  }

  vk::SurfaceFormatKHR pick_surface_format( const vk::raii::PhysicalDevice & physical_device, const vk::raii::SurfaceKHR & surface )
  {
    auto surface_formats = physical_device.getSurfaceFormatsKHR( *surface );
    auto surface_format  = surface_formats[0];

    for ( const auto & fmt : surface_formats )
    {
      if ( fmt.format == vk::Format::eB8G8R8A8Srgb && fmt.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear )
      {
        surface_format = fmt;
      }
    }
    return surface_format;
  }

  vk::Extent2D pick_extent( const vk::raii::PhysicalDevice & physical_device, const vk::raii::SurfaceKHR & surface, std::pair<uint32_t, uint32_t> window_size )
  {
    auto         surface_capabilities = physical_device.getSurfaceCapabilitiesKHR( *surface );
    vk::Extent2D extent;
    if ( surface_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max() )
    {
      extent = surface_capabilities.currentExtent;
    }
    else
    {
      auto [w, h]   = window_size;
      extent.width  = std::clamp( static_cast<uint32_t>( w ), surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width );
      extent.height = std::clamp( static_cast<uint32_t>( h ), surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height );
    }

    return extent;
  }

  size_t get_image_count( const vk_hardware & hardware, const vk::raii::SurfaceKHR & surface )
  {
    auto surface_capabilities = hardware.physical_device().getSurfaceCapabilitiesKHR( *surface );
    auto image_count          = surface_capabilities.minImageCount + 1;
    if ( surface_capabilities.maxImageCount > 0 )
    {
      image_count = std::min( surface_capabilities.maxImageCount, image_count );
    }
    return image_count;
  }

  vk::raii::SwapchainKHR build_swapchian( const vk_hardware &          hardware,
                                          const vk::raii::SurfaceKHR & surface,
                                          size_t                       image_count,
                                          vk::PresentModeKHR           present_mode,
                                          const vk::SurfaceFormatKHR & surface_format,
                                          vk::Extent2D                 extent )
  {
    auto surface_capabilities = hardware.physical_device().getSurfaceCapabilitiesKHR( *surface );

    std::array<uint32_t, 2>      queue_family_indices{ *hardware.queue_index( QueueType::GRAPHICS ),
                                                  *hardware.queue_index( QueueType::PRESENT, std::cref( surface ) ) };
    std::unordered_set<uint32_t> unique_queue_family_indices{ *hardware.queue_index( QueueType::GRAPHICS ),
                                                              *hardware.queue_index( QueueType::PRESENT, std::cref( surface ) ) };

    auto sharing_mode = unique_queue_family_indices.size() == 1 ? vk::SharingMode::eExclusive : vk::SharingMode::eConcurrent;

    vk::SwapchainCreateInfoKHR swapchain_create_info{ .surface               = *surface,
                                                      .minImageCount         = static_cast<uint32_t>( image_count ),
                                                      .imageFormat           = surface_format.format,
                                                      .imageColorSpace       = surface_format.colorSpace,
                                                      .imageExtent           = extent,
                                                      .imageArrayLayers      = 1,
                                                      .imageUsage            = vk::ImageUsageFlagBits::eColorAttachment,
                                                      .imageSharingMode      = sharing_mode,
                                                      .queueFamilyIndexCount = static_cast<uint32_t>( unique_queue_family_indices.size() ),
                                                      .pQueueFamilyIndices   = queue_family_indices.data(),
                                                      .preTransform          = surface_capabilities.currentTransform,
                                                      .compositeAlpha        = vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                                      .presentMode           = present_mode,
                                                      .clipped               = VK_TRUE,
                                                      .oldSwapchain          = nullptr };

    return vk::raii::SwapchainKHR( hardware.device(), swapchain_create_info );
  }

  std::vector<vk::raii::ImageView>
    build_image_views( const vk_hardware & hardware, const vk::SurfaceFormatKHR & surface_format, const vk::raii::SwapchainKHR & swapchain )
  {
    std::vector<vk::raii::ImageView> image_views;
    for ( auto & image : swapchain.getImages() )
    {
      vk::ImageViewCreateInfo create_info{
        .image      = image,
        .viewType   = vk::ImageViewType::e2D,
        .format     = surface_format.format,
        .components = vk::ComponentMapping(),
        .subresourceRange{ .aspectMask = vk::ImageAspectFlagBits::eColor, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1 }
      };
      image_views.emplace_back( hardware.device(), create_info );
    }
    return image_views;
  }

  vk_swapchain::vk_swapchain( const vk_hardware & hardware, const vk::raii::SurfaceKHR & surface, std::pair<uint32_t, uint32_t> window_size )
    : present_mode_( pick_present_mode( hardware.physical_device(), surface ) )
    , surface_format_( pick_surface_format( hardware.physical_device(), surface ) )
    , extent_( pick_extent( hardware.physical_device(), surface, window_size ) )
    , image_count_( get_image_count( hardware, surface ) )
    , max_frames_in_flight_( std::min( image_count_, size_t( 2 ) ) )
    , swapchain_( build_swapchian( hardware, surface, image_count_, present_mode_, surface_format_, extent_ ) )
    , image_views_( build_image_views( hardware, surface_format_, swapchain_ ) )
  {
  }

  uint32_t vk_swapchain::acquire_next_image( const vk::raii::Semaphore & image_available_semaphore )
  {
    auto [result, index] = swapchain_.acquireNextImage( UINT64_MAX, *image_available_semaphore );
    switch ( result )
    {
      case vk::Result::eSuccess: break;
      case vk::Result::eSuboptimalKHR: break;
      default: throw vk::SystemError( vk::make_error_code( result ), "swapchain::acquireNextImage" );
    }
    return index;
  }

}  // namespace wabby::render::vulkan