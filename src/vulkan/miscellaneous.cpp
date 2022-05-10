#include "miscellaneous.hpp"

namespace wawy::vulkan::misc
{

wawy::util::logger &get_logger()
{
    static wawy::util::logger logger = []{
        wawy::util::logger lg;
        lg.add_console_sink();
        return lg;
    }();

    return logger;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* data,
    void* userdata)
{
    vk::DebugUtilsMessageTypeFlagsEXT type(message_severity);
    vk::DebugUtilsMessageSeverityFlagsEXT severity(message_severity);

    WLOG(get_logger(), debug, "{} {} {}", vk::to_string(severity), vk::to_string(type), data->pMessage);
    return VK_FALSE;
}

}