#ifndef _WABBY_VULKAN_DEFINES_HPP
#define _WABBY_VULKAN_DEFINES_HPP

// std
#include "string"
#include "string_view"

namespace wabby::render::vulkan
{

  constexpr std::string_view EXT_NAME_VK_KHR_portability_subset              = "VK_KHR_portability_subset";
  constexpr std::string_view EXT_NAME_VK_KHR_get_physical_device_properties2 = "VK_KHR_get_physical_device_properties2";
  constexpr std::string_view EXT_NAME_VK_EXT_debug_utils                     = "VK_EXT_debug_utils";
  constexpr std::string_view LAYER_NAME_VK_LAYER_KHRONOS_validation          = "VK_LAYER_KHRONOS_validation";
  constexpr std::string_view EXT_NAME_VK_KHR_swapchain                       = "VK_KHR_swapchain";
  constexpr std::string_view EXT_NAME_VK_KHR_shader_non_semantic_info        = "VK_KHR_shader_non_semantic_info";
  constexpr std::string_view EXT_NAME_VK_KHR_portability_enumeration         = "VK_KHR_portability_enumeration";

  constexpr std::string_view WABBY_ENGINE_NAME = "wabby";

  enum class QueueType
  {
    PRESENT,
    GRAPHICS,
    COMPUTE,
    TRANSFER
  };

}  // namespace wabby::render::vulkan

namespace vk
{
  inline std::string to_string( wabby::render::vulkan::QueueType type )
  {
    switch ( type )
    {
      case wabby::render::vulkan::QueueType::PRESENT: return "Present";
      case wabby::render::vulkan::QueueType::GRAPHICS: return "Graphics";
      case wabby::render::vulkan::QueueType::COMPUTE: return "Compute";
      case wabby::render::vulkan::QueueType::TRANSFER: return "Transfer";
    }

    return "";
  };
}  // namespace vk

#endif