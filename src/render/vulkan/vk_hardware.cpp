#include "vk_hardware.hpp"

// spdlog
#include "spdlog/spdlog.h"

namespace wabby::render::vulkan
{
  namespace detail
  {
    std::optional<uint32_t> present_queue_index( const vk::PhysicalDevice & physical_device, const vk::SurfaceKHR & surface )
    {
      auto queue_family_properties = physical_device.getQueueFamilyProperties<vk_allocator<vk::QueueFamilyProperties>>();
      for ( uint32_t i = 0; i < queue_family_properties.size(); i++ )
      {
        if ( physical_device.getSurfaceSupportKHR( i, surface ) )
        {
          return i;
        }
      }

      return {};
    }

    std::optional<uint32_t> first_queue_index( const vk::PhysicalDevice & physical_device, vk::QueueFlags type )
    {
      auto queue_family_properties = physical_device.getQueueFamilyProperties<vk_allocator<vk::QueueFamilyProperties>>();
      for ( uint32_t i = 0; i < queue_family_properties.size(); i++ )
      {
        if ( queue_family_properties[i].queueFlags & type )
        {
          return i;
        }
      }

      return {};
    }

  }  // namespace detail

  vk_queue_cache::vk_queue_cache( const vk_hardware & hardware, const vk::SurfaceKHR & surface ) : hardware_( hardware ), surface_( surface ) {}

  std::optional<vk::raii::Queue> vk_queue_cache::queue( QueueType type )
  {
    if ( auto cache_item = cache_.find( type ); cache_item != cache_.end() )
    {
      return hardware_.device().getQueue( cache_item->second, 0 );
    }

    auto index = queue_index( type );
    if ( !index.has_value() )
    {
      return {};
    }

    return hardware_.device().getQueue( *index, 0 );
  }

  std::optional<uint32_t> vk_queue_cache::queue_index( QueueType type )
  {
    if ( auto cache_item = cache_.find( type ); cache_item != cache_.end() )
    {
      return cache_item->second;
    }

    std::optional<uint32_t> index;
    switch ( type )
    {
      case QueueType::PRESENT: index = detail::present_queue_index( *hardware_.physical_device(), surface_ ); break;
      case QueueType::GRAPHICS: index = detail::first_queue_index( *hardware_.physical_device(), vk::QueueFlagBits::eGraphics ); break;
      case QueueType::COMPUTE: throw std::runtime_error( "unsupported now" ); break;
      case QueueType::TRANSFER: throw std::runtime_error( "unsupported now" ); break;
    }

    if ( index.has_value() )
    {
      cache_.emplace( type, *index );
    }

    return index;
  }

  vk_hardware::vk_hardware( const vk::raii::Instance & instance, const vk::SurfaceKHR & surface )
    : physical_device_( pick_physical_device_( instance, surface ) ), device_( build_device_() )
  {
  }

  const vk::raii::PhysicalDevice & vk_hardware::physical_device() const
  {
    return physical_device_;
  }

  const vk::raii::Device & vk_hardware::device() const
  {
    return device_;
  }

  vk_vector<vk_string> vk_hardware::check_extensions_supported_( const vk::PhysicalDevice &          physical_device,
                                                                 const vk_vector<std::string_view> & desired_extensions )
  {
    auto extension_props = physical_device.enumerateDeviceExtensionProperties<vk_allocator<vk::ExtensionProperties>>();

    vk_unordered_set<std::string_view> extensions;

    auto prop_to_name = []( auto && prop ) { return prop.extensionName.data(); };
    std::transform( extension_props.begin(), extension_props.end(), std::inserter( extensions, extensions.end() ), prop_to_name );

    vk_vector<vk_string> result;
    result.reserve( desired_extensions.size() );
    for ( auto extension : desired_extensions )
    {
      if ( extensions.contains( extension ) )
      {
        result.push_back( extension.data() );
      }
    }
    return result;
  }

  vk::raii::PhysicalDevice vk_hardware::pick_physical_device_( const vk::raii::Instance & instance, const vk::SurfaceKHR & surface )
  {
    const vk_vector<std::string_view> REQUIRED_DEVICE_EXTENSION{ EXT_NAME_VK_KHR_swapchain };
    const vk_vector<std::string_view> OPTIONAL_DEVICE_EXTENSION{ EXT_NAME_VK_KHR_portability_subset };

    for ( auto & physical_device : ( *instance ).enumeratePhysicalDevices<vk_allocator<vk::PhysicalDevice>>() )
    {
      auto properties = physical_device.getProperties();
      auto features   = physical_device.getFeatures();

      if ( properties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu )
      {
        continue;
      }

      if ( !features.tessellationShader )
      {
        continue;
      }

      if ( properties.apiVersion < VK_API_VERSION_1_1 )
      {
        continue;
      }

      auto required_extensions = check_extensions_supported_( physical_device, REQUIRED_DEVICE_EXTENSION );
      if ( required_extensions.size() != REQUIRED_DEVICE_EXTENSION.size() )
      {
        continue;
      }

      auto present_modes = physical_device.getSurfacePresentModesKHR<vk_allocator<vk::PresentModeKHR>>( surface );
      if ( present_modes.empty() )
      {
        continue;
      }

      auto surface_formats = physical_device.getSurfaceFormatsKHR<vk_allocator<vk::SurfaceFormatKHR>>( surface );
      if ( surface_formats.empty() )
      {
        continue;
      }

      auto present_queue_index = detail::present_queue_index( physical_device, surface );
      if ( !present_queue_index.has_value() )
      {
        continue;
      }

      auto graphics_queue_index = detail::first_queue_index( physical_device, vk::QueueFlagBits::eGraphics );
      if ( !graphics_queue_index.has_value() )
      {
        continue;
      }

      return vk::raii::PhysicalDevice( instance, physical_device );
    }

    throw std::runtime_error( "no suitable gpu found" );
  }

  void vk_hardware::wait_fences( const vk::ArrayProxy<const vk::Fence> & fences, bool wait_all, uint64_t timeout )
  {
    auto result = device_.waitForFences( fences, wait_all, timeout );
    if ( result == vk::Result::eTimeout )
    {
      throw vk::SystemError( vk::make_error_code( result ), "device::waitForFences" );
    }
  }

  void vk_hardware::wait_fences( vk::ArrayProxy<const vk::raii::Fence> & fences, bool wait_all, uint64_t timeout )
  {
    vk_vector<const vk::Fence> tmp_fences;
    tmp_fences.reserve( fences.size() );
    for ( const auto & fence : fences )
    {
      tmp_fences.push_back( *fence );
    }

    auto result = device_.waitForFences( tmp_fences, wait_all, timeout );
    if ( result == vk::Result::eTimeout )
    {
      throw vk::SystemError( vk::make_error_code( result ), "device::waitForFences" );
    }
  };

  vk::raii::Device vk_hardware::build_device_()
  {
    const vk_vector<std::string_view> REQUIRED_DEVICE_EXTENSION{ EXT_NAME_VK_KHR_swapchain };
    const vk_vector<std::string_view> OPTIONAL_DEVICE_EXTENSION{ EXT_NAME_VK_KHR_portability_subset };

    auto required_extensions = check_extensions_supported_( *physical_device_, REQUIRED_DEVICE_EXTENSION );
    auto optional_extensions = check_extensions_supported_( *physical_device_, OPTIONAL_DEVICE_EXTENSION );

    std::vector<const char *> enable_extensions;
    std::transform( required_extensions.begin(), required_extensions.end(), std::back_inserter( enable_extensions ), std::mem_fn( &vk_string::c_str ) );
    std::transform( optional_extensions.begin(), optional_extensions.end(), std::back_inserter( enable_extensions ), std::mem_fn( &vk_string::c_str ) );

    auto                                   queue_families = ( *physical_device_ ).getQueueFamilyProperties<vk_allocator<vk::QueueFamilyProperties>>();
    std::vector<vk::DeviceQueueCreateInfo> device_queue_create_infos;
    std::vector<float>                     device_queue_priorities( queue_families.size(), 1.f );
    for ( uint32_t i = 0; i < queue_families.size(); i++ )
    {
      device_queue_create_infos.push_back( { .queueFamilyIndex = i, .queueCount = 1, .pQueuePriorities = &device_queue_priorities[i] } );
    }

    vk::PhysicalDeviceFeatures physical_device_features{};

    vk::DeviceCreateInfo device_create_info{ .queueCreateInfoCount    = static_cast<uint32_t>( device_queue_create_infos.size() ),
                                             .pQueueCreateInfos       = device_queue_create_infos.data(),
                                             .enabledLayerCount       = 0,
                                             .ppEnabledLayerNames     = nullptr,
                                             .enabledExtensionCount   = static_cast<uint32_t>( enable_extensions.size() ),
                                             .ppEnabledExtensionNames = enable_extensions.data(),
                                             .pEnabledFeatures        = &physical_device_features };

    return vk::raii::Device( physical_device_, device_create_info );
  }

}  // namespace wabby::render::vulkan