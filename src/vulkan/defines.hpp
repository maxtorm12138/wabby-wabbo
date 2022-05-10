#ifndef _WAWY_VULKAN_DEFINES_HPP
#define _WAWY_VULKAN_DEFINES_HPP

// std
#include "string_view"

namespace wawy::vulkan
{
constexpr std::string_view EXT_NAME_VK_KHR_portability_subset = "VK_KHR_portability_subset";
constexpr std::string_view EXT_NAME_VK_KHR_get_physical_device_properties2 = "VK_KHR_get_physical_device_properties2";
constexpr std::string_view EXT_NAME_VK_EXT_debug_utils = "VK_EXT_debug_utils";
constexpr std::string_view LAYER_NAME_VK_LAYER_KHRONOS_validation = "VK_LAYER_KHRONOS_validation";
constexpr std::string_view EXT_NAME_VK_KHR_swapchain = "VK_KHR_swapchain";
constexpr std::string_view EXT_NAME_VK_KHR_shader_non_semantic_info = "VK_KHR_shader_non_semantic_info";
constexpr std::string_view EXT_NAME_VK_KHR_portability_enumeration = "VK_KHR_portability_enumeration";
constexpr auto str_to_c_str = [](const std::string &str) { return str.c_str(); };

}

#endif