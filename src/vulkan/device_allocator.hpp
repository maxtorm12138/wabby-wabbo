#ifndef _WABBY_VULKAN_ALLOCATOR_HPP
#define _WABBY_VULKAN_ALLOCATOR_HPP

// module
#include "util/noncopyable.hpp"

// vulkan
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

// vma
#include "vk_mem_alloc.h"

namespace wabby::vulkan
{

class device_allocator : public wabby::util::noncopyable
{
public:
    device_allocator(const vk::raii::Instance &instance, const vk::raii::PhysicalDevice &physical_device, const vk::raii::Device &device);
    ~device_allocator();

public:
    operator VmaAllocator &() { return allocator_; }
    operator const VmaAllocator &() const { return allocator_; }

    VmaAllocator & operator*() { return allocator_; }
    const VmaAllocator & operator*() const { return allocator_; }

    VmaAllocator &get() { return allocator_; }
    const VmaAllocator &get() const { return allocator_; }

private:
    VmaAllocator allocator_{VK_NULL_HANDLE};
};

}
#endif