#include "vk_device_allocator.hpp"

// vma
#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

namespace wabby::render::vulkan
{

vk_device_allocator::vk_device_allocator(const vk::raii::Instance &instance, const vk::raii::PhysicalDevice &physical_device, const vk::raii::Device &device)
{
    VmaAllocatorCreateInfo allocator_create_info
    {
        .physicalDevice = *physical_device,
        .device = *device,
        .instance = *instance,
        .vulkanApiVersion = VK_API_VERSION_1_1
    };

    auto result = static_cast<vk::Result>(vmaCreateAllocator(&allocator_create_info, &allocator_));
    if (result != vk::Result::eSuccess)
    {
        spdlog::get("vulkan")->error("device_allocator call vmaCreateAllocator {}", vk::to_string(result));
        throw vk::SystemError(vk::make_error_code(result), "vmaCreateAllocator");
    }
}

vk_device_allocator::~vk_device_allocator()
{
    if (allocator_ != VK_NULL_HANDLE)
    {
        vmaDestroyAllocator(allocator_);
    }
}
}