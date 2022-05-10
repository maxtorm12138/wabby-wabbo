#include "instance.hpp"

// module
#include "defines.hpp"
#include "miscellaneous.hpp"

#include "util/log.hpp"

// std
#include "unordered_set"
#include "algorithm"

namespace wawy::vulkan::instance
{


vk::raii::Instance build(
    const vk::raii::Context &context,
    const vk::ApplicationInfo &application_info,
    const std::vector<std::string> &desired_extensions)
{
    #ifndef NDEBUG
    std::vector<const char *> REQUIRED_LAYERS{ LAYER_NAME_VK_LAYER_KHRONOS_validation.data() };
    std::vector<const char *> REQUIRED_EXTENSIONS{ EXT_NAME_VK_EXT_debug_utils.data() };
    #else
    std::vector<const char *> REQUIRED_LAYERS {};
    std::vector<const char *> REQUIRED_EXTENSIONS {};
    #endif

    std::unordered_set<std::string_view> OPTIONAL_LAYERS {};
    std::unordered_set<std::string_view> OPTIONAL_EXTENSIONS{ EXT_NAME_VK_KHR_get_physical_device_properties2.data(), EXT_NAME_VK_KHR_portability_enumeration.data() };
    std::transform(desired_extensions.begin(), desired_extensions.end(), std::back_inserter(REQUIRED_EXTENSIONS), std::mem_fn(&std::string::c_str));

    auto enable_layers = REQUIRED_LAYERS;
    // check optional layers
    auto layer_properties = context.enumerateInstanceLayerProperties();
    std::vector<const char *> layers;
    std::transform(layer_properties.begin(), layer_properties.end(), std::back_inserter(layers), [](auto &&prop) { return prop.layerName.data(); });
    std::copy_if(layers.begin(), layers.end(), std::back_inserter(enable_layers), [&](auto &&name) { return OPTIONAL_LAYERS.contains(name); });

    auto enable_extensions = REQUIRED_EXTENSIONS;
    // check optional extensions
    auto extension_properties = context.enumerateInstanceExtensionProperties();
    std::vector<const char *> extensions;
    std::transform(extension_properties.begin(), extension_properties.end(), std::back_inserter(extensions), [](auto &&prop) { return prop.extensionName.data(); });
    std::copy_if(extensions.begin(), extensions.end(), std::back_inserter(enable_extensions), [&](auto &&extension) { return OPTIONAL_EXTENSIONS.contains(extension); });


    vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT> chain
    {
        vk::InstanceCreateInfo
        {
            .flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR,
            .pApplicationInfo = &application_info,
            .enabledLayerCount = static_cast<uint32_t>(enable_layers.size()),
            .ppEnabledLayerNames = enable_layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(enable_extensions.size()),
            .ppEnabledExtensionNames = enable_extensions.data()
        },
        vk::DebugUtilsMessengerCreateInfoEXT
        {
            misc::DEBUG_MESSENGER_CREATE_INFO
        }
    };

    #ifdef NDEBUG
    chain.unlink<vk::DebugUtilsMessengerCreateInfoEXT>();
    #endif

    return vk::raii::Instance(context, chain.get<vk::InstanceCreateInfo>());
}
}