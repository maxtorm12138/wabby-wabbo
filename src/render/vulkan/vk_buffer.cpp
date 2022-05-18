#include "vk_buffer.hpp"

// fmt
#include "fmt/format.h"

namespace wabby::render::vulkan
{

  buffer::buffer( const vk_device_allocator & allocator, vk::BufferCreateInfo buffer_create_info, VmaAllocationCreateInfo allocation_create_info )
    : allocator_( allocator )
  {
    VkBuffer buffer;
    auto     result = vmaCreateBuffer( allocator_.get(),
                                   reinterpret_cast<vk::BufferCreateInfo::NativeType *>( &buffer_create_info ),
                                   &allocation_create_info,
                                   &buffer,
                                   &allocation_,
                                   &allocation_info_ );

    if ( result != VK_SUCCESS )
    {
      throw std::runtime_error( fmt::format( "vmaCreateBuffer fail result: {}", result ) );
    }
    buffer_ = vk::Buffer( buffer );
  }

  buffer::buffer( buffer && other ) noexcept
    : allocator_( other.allocator_ )
    , buffer_( std::exchange( other.buffer_, buffer_ ) )
    , allocation_( std::exchange( other.allocation_, allocation_ ) )
    , allocation_info_( std::exchange( other.allocation_info_, allocation_info_ ) )
  {
  }

  buffer & buffer::operator=( buffer && other ) noexcept
  {
    allocator_ = other.allocator_;
    std::swap( this->buffer_, other.buffer_ );
    std::swap( this->allocation_, other.allocation_ );
    std::swap( this->allocation_info_, other.allocation_info_ );
    return *this;
  }

  buffer::~buffer() noexcept
  {
    if ( allocation_ != VK_NULL_HANDLE )
    {
      vmaDestroyBuffer( allocator_.get(), buffer_, allocation_ );
    }
  }

  void * buffer::map() const
  {
    void * data;
    auto   result = vmaMapMemory( allocator_.get(), allocation_, &data );
    if ( result != VK_SUCCESS )
    {
      throw std::runtime_error( fmt::format( "vmaMapMemory fail result: {}", result ) );
    }
    return data;
  }

  void buffer::unmap() const
  {
    vmaUnmapMemory( allocator_.get(), allocation_ );
  }

}  // namespace wabby::render::vulkan