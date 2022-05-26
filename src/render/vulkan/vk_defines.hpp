#ifndef _WABBY_VULKAN_DEFINES_HPP
#define _WABBY_VULKAN_DEFINES_HPP

// std
#include "string"
#include "string_view"

// engine version
#include "wabby/api.hpp"

// vulkan api version
#include "vulkan/vulkan.h"

namespace wabby::render::vulkan
{

  constexpr std::string_view EXT_NAME_VK_KHR_portability_subset              = "VK_KHR_portability_subset";
  constexpr std::string_view EXT_NAME_VK_KHR_get_physical_device_properties2 = "VK_KHR_get_physical_device_properties2";
  constexpr std::string_view EXT_NAME_VK_EXT_debug_utils                     = "VK_EXT_debug_utils";
  constexpr std::string_view LAYER_NAME_VK_LAYER_KHRONOS_validation          = "VK_LAYER_KHRONOS_validation";
  constexpr std::string_view EXT_NAME_VK_KHR_swapchain                       = "VK_KHR_swapchain";
  constexpr std::string_view EXT_NAME_VK_KHR_shader_non_semantic_info        = "VK_KHR_shader_non_semantic_info";
  constexpr std::string_view EXT_NAME_VK_KHR_portability_enumeration         = "VK_KHR_portability_enumeration";

  constexpr std::string_view WABBY_ENGINE_NAME           = "wabby";
  constexpr uint32_t         WABBY_ENGINE_VERSION        = WABBY_API_MAKE_VERSION( 1, 0, 0 );
  constexpr uint32_t         WABBY_ENGINE_VULKAN_VERSION = VK_API_VERSION_1_1;

}  // namespace wabby::render::vulkan

#endif