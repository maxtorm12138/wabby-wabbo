#ifndef _WABBY_VULKAN_ALLOCATOR_HPP
#define _WABBY_VULKAN_ALLOCATOR_HPP

// boost
#include "boost/noncopyable.hpp"

// vulkan
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

// vma
#include "vk_mem_alloc.h"

namespace wabby::render::vulkan
{

class vk_device_allocator : public boost::noncopyable
{
public:
    vk_device_allocator(const vk::raii::Instance &instance, const vk::raii::PhysicalDevice &physical_device, const vk::raii::Device &device);
    ~vk_device_allocator();

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