#ifndef _WAWY_VULKAN_MISCELLANEOUS_HPP
#define _WAWY_VULKAN_MISCELLANEOUS_HPP

// module
#include "util/log.hpp"

// std
#include "string"

// vulkan
#include "vulkan/vulkan.hpp"

namespace wawy::vulkan
{
    using namespace wawy::util::log_severity;
}

namespace wawy::vulkan::misc
{

wawy::util::logger &get_logger();

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* data,
    void* userdata);


const vk::DebugUtilsMessengerCreateInfoEXT DEBUG_MESSENGER_CREATE_INFO
{
    .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                       vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
                       vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
                       vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,

    .messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                   vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                   vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
    
    .pfnUserCallback = &debug_callback
};

}

#endif