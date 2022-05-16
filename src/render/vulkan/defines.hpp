#ifndef _WABBY_VULKAN_DEFINES_HPP
#define _WABBY_VULKAN_DEFINES_HPP

// std
#include "string_view"

namespace wabby::vulkan
{

constexpr std::string_view EXT_NAME_VK_KHR_portability_subset = "VK_KHR_portability_subset";
constexpr std::string_view EXT_NAME_VK_KHR_get_physical_device_properties2 = "VK_KHR_get_physical_device_properties2";
constexpr std::string_view EXT_NAME_VK_EXT_debug_utils = "VK_EXT_debug_utils";
constexpr std::string_view LAYER_NAME_VK_LAYER_KHRONOS_validation = "VK_LAYER_KHRONOS_validation";
constexpr std::string_view EXT_NAME_VK_KHR_swapchain = "VK_KHR_swapchain";
constexpr std::string_view EXT_NAME_VK_KHR_shader_non_semantic_info = "VK_KHR_shader_non_semantic_info";
constexpr std::string_view EXT_NAME_VK_KHR_portability_enumeration = "VK_KHR_portability_enumeration";

constexpr std::string_view WABBY_ENGINE_NAME = "wabby-wabbo";

enum class QueueType { PRESENT, GRAPHICS, COMPUTE, TRANSFER };

}

#endif