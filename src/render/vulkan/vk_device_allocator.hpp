#ifndef _WABBY_VULKAN_ALLOCATOR_HPP
#define _WABBY_VULKAN_ALLOCATOR_HPP

// vma
#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-copy"
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
#include "vk_mem_alloc.h"
#ifdef __GNUC__
#  pragma GCC diagnostic pop
#endif

// comm headers
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{

  class vk_device_allocator : public boost::noncopyable
  {
  public:
    vk_device_allocator( const vk::Instance & instance, const vk::PhysicalDevice & physical_device, const vk::Device & device );
    ~vk_device_allocator();

  public:
    operator VmaAllocator &()
    {
      return allocator_;
    }

    operator const VmaAllocator &() const
    {
      return allocator_;
    }

    VmaAllocator & operator*()
    {
      return allocator_;
    }

    const VmaAllocator & operator*() const
    {
      return allocator_;
    }

    VmaAllocator & get()
    {
      return allocator_;
    }

    const VmaAllocator & get() const
    {
      return allocator_;
    }

  private:
    VmaAllocator allocator_{ VK_NULL_HANDLE };
  };

}  // namespace wabby::render::vulkan
#endif