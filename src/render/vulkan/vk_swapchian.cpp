#include "vk_swapchian.hpp"

// spdlog
#include "spdlog/spdlog.h"

// std
#include "unordered_set"

namespace wabby::render::vulkan
{

  vk_swapchain::vk_swapchain( const vk_hardware &                           hardware,
                              const vk::raii::SurfaceKHR &                  surface,
                              vk_queue_cache &                              queue_cache,
                              std::function<void( uint32_t *, uint32_t * )> fn_get_window_size )
    : fn_get_window_size_( fn_get_window_size )
    , present_mode_( pick_present_mode( hardware.physical_device(), surface ) )
    , surface_format_( pick_surface_format( hardware.physical_device(), surface ) )
    , extent_( pick_extent( hardware.physical_device(), surface ) )
    , image_count_( get_image_count( hardware, surface ) )
    , max_frames_in_flight_( std::min( image_count_, size_t( 2 ) ) )
    , swapchain_( build_swapchian( hardware, surface, queue_cache ) )
    , image_views_( build_image_views( hardware ) )
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

  void vk_swapchain::recreate( const vk_hardware & hardware, const vk::raii::SurfaceKHR & surface, vk_queue_cache & queue_cache )
  {
    present_mode_         = pick_present_mode( hardware.physical_device(), surface );
    surface_format_       = pick_surface_format( hardware.physical_device(), surface );
    extent_               = pick_extent( hardware.physical_device(), surface );
    image_count_          = get_image_count( hardware, surface );
    max_frames_in_flight_ = std::min( image_count_, size_t( 2 ) );
    swapchain_            = std::move( build_swapchian( hardware, surface, queue_cache, true ) );
    image_views_.clear();
    auto new_image_views = build_image_views( hardware );
    for ( auto & img_v : new_image_views )
    {
      image_views_.push_back( std::move( img_v ) );
    }
  }

  vk::PresentModeKHR vk_swapchain::pick_present_mode( const vk::raii::PhysicalDevice & physical_device, const vk::raii::SurfaceKHR & surface )
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

  vk::SurfaceFormatKHR vk_swapchain::pick_surface_format( const vk::raii::PhysicalDevice & physical_device, const vk::raii::SurfaceKHR & surface )
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

  vk::Extent2D vk_swapchain::pick_extent( const vk::raii::PhysicalDevice & physical_device, const vk::raii::SurfaceKHR & surface )
  {
    auto surface_capabilities = physical_device.getSurfaceCapabilitiesKHR( *surface );

    vk::Extent2D extent;
    if ( surface_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max() )
    {
      extent = surface_capabilities.currentExtent;
    }
    else
    {
      uint32_t w{ 0 }, h{ 0 };
      fn_get_window_size_( &w, &h );

      extent.width  = std::clamp( static_cast<uint32_t>( w ), surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width );
      extent.height = std::clamp( static_cast<uint32_t>( h ), surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height );
    }

    return extent;
  }

  size_t vk_swapchain::get_image_count( const vk_hardware & hardware, const vk::raii::SurfaceKHR & surface )
  {
    auto surface_capabilities = hardware.physical_device().getSurfaceCapabilitiesKHR( *surface );
    auto image_count          = surface_capabilities.minImageCount + 1;
    if ( surface_capabilities.maxImageCount > 0 )
    {
      image_count = std::min( surface_capabilities.maxImageCount, image_count );
    }
    return image_count;
  }

  vk::raii::SwapchainKHR
    vk_swapchain::build_swapchian( const vk_hardware & hardware, const vk::raii::SurfaceKHR & surface, vk_queue_cache & queue_cache, bool recreate )
  {
    auto surface_capabilities = hardware.physical_device().getSurfaceCapabilitiesKHR( *surface );

    std::array<uint32_t, 2>      queue_family_indices{ *queue_cache.queue_index( QueueType::GRAPHICS ), *queue_cache.queue_index( QueueType::PRESENT ) };
    std::unordered_set<uint32_t> unique_queue_family_indices{ *queue_cache.queue_index( QueueType::GRAPHICS ), *queue_cache.queue_index( QueueType::PRESENT ) };

    auto sharing_mode = unique_queue_family_indices.size() == 1 ? vk::SharingMode::eExclusive : vk::SharingMode::eConcurrent;

    vk::SwapchainCreateInfoKHR swapchain_create_info{ .surface               = *surface,
                                                      .minImageCount         = static_cast<uint32_t>( image_count_ ),
                                                      .imageFormat           = surface_format_.format,
                                                      .imageColorSpace       = surface_format_.colorSpace,
                                                      .imageExtent           = extent_,
                                                      .imageArrayLayers      = 1,
                                                      .imageUsage            = vk::ImageUsageFlagBits::eColorAttachment,
                                                      .imageSharingMode      = sharing_mode,
                                                      .queueFamilyIndexCount = static_cast<uint32_t>( unique_queue_family_indices.size() ),
                                                      .pQueueFamilyIndices   = queue_family_indices.data(),
                                                      .preTransform          = surface_capabilities.currentTransform,
                                                      .compositeAlpha        = vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                                      .presentMode           = present_mode_,
                                                      .clipped               = VK_TRUE,
                                                      .oldSwapchain          = recreate ? *swapchain_ : nullptr };

    return vk::raii::SwapchainKHR( hardware.device(), swapchain_create_info );
  }

  vk_vector<vk::raii::ImageView> vk_swapchain::build_image_views( const vk_hardware & hardware )
  {
    vk_vector<vk::raii::ImageView> image_views;
    for ( auto & image : swapchain_.getImages() )
    {
      vk::ImageViewCreateInfo create_info{
        .image      = image,
        .viewType   = vk::ImageViewType::e2D,
        .format     = surface_format_.format,
        .components = vk::ComponentMapping(),
        .subresourceRange{ .aspectMask = vk::ImageAspectFlagBits::eColor, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1 }
      };
      image_views.emplace_back( hardware.device(), create_info );
    }
    return image_views;
  }

}  // namespace wabby::render::vulkan