#ifndef _WABBY_VULKAN_BUFFER_HPP
#define _WABBY_VULKAN_BUFFER_HPP

// module
#include "vk_device_allocator.hpp"

// vma
#include "vk_mem_alloc.h"

// comm headers
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{

class buffer : public boost::noncopyable
{
public:
    buffer(const vk_device_allocator &allocator, vk::BufferCreateInfo buffer_create_info, VmaAllocationCreateInfo allocation_create_info);

    buffer(buffer &&other) noexcept;

    buffer &operator=(buffer &&other) noexcept;

    ~buffer() noexcept;

public:
    void *map() const;

    void unmap() const;

private:
    std::reference_wrapper<const vk_device_allocator> allocator_;
    VmaAllocation allocation_{VK_NULL_HANDLE};
    VmaAllocationInfo allocation_info_;
    vk::Buffer buffer_;
};
}

#endif