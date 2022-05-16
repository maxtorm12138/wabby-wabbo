#include "device_allocator.hpp"

// module
#include "defines.hpp"

// vma
#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

// fmt
#include "fmt/format.h"

namespace wabby::vulkan
{

device_allocator::device_allocator(const vk::raii::Instance &instance, const vk::raii::PhysicalDevice &physical_device, const vk::raii::Device &device)
{
    VmaAllocatorCreateInfo allocator_create_info
    {
        .physicalDevice = *physical_device,
        .device = *device,
        .instance = *instance,
        .vulkanApiVersion = VK_API_VERSION_1_1
    };

    auto result = vmaCreateAllocator(&allocator_create_info, &allocator_);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error(fmt::format("vmaCreateAllocator fail result: {}", result));
    }
}

device_allocator::~device_allocator()
{
    if (allocator_ != VK_NULL_HANDLE)
    {
        vmaDestroyAllocator(allocator_);
    }
}
}