#ifndef WABBY_RENDER_VULKAN_ALLOCATOR_HPP
#define WABBY_RENDER_VULKAN_ALLOCATOR_HPP

// std
#include "bit"

// module
#include "vk_globals.hpp"

namespace wabby::render::vulkan
{
  template <typename T>
  class vk_allocator
  {
  public:
    constexpr vk_allocator() noexcept {}

    constexpr vk_allocator( const vk_allocator & ) noexcept {}

    template <class U>
    constexpr vk_allocator( const vk_allocator<U> & ) noexcept
    {
    }

  public:
    [[nodiscard]] constexpr T * allocate( size_t n )
    {
      return static_cast<T *>( global::allocation( sizeof( T ) * n, std::bit_ceil( sizeof( T ) ) ) );
    }

    constexpr void deallocate( T * p, size_t )
    {
      global::free( p );
    }

  private:
  };

}  // namespace wabby::render::vulkan

#endif