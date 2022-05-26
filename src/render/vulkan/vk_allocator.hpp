#ifndef WABBY_RENDER_VULKAN_ALLOCATOR_HPP
#define WABBY_RENDER_VULKAN_ALLOCATOR_HPP

// std
#include "bit"

// module
#include "vk_globals.hpp"

namespace wabby::render::vulkan
{

  template <typename T, size_t Alignment = sizeof( void * )>
  class vk_allocator
  {
  public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = std::ptrdiff_t;

    template <class U>
    struct rebind
    {
      typedef vk_allocator<U> other;
    };

    constexpr vk_allocator() noexcept {}

    constexpr vk_allocator( const vk_allocator & ) noexcept {}

    template <class U>
    constexpr vk_allocator( const vk_allocator<U> & ) noexcept
    {
    }

  public:
    [[nodiscard]] constexpr T * allocate( size_t n )
    {
      return static_cast<T *>( global::allocation( sizeof( T ) * n, Alignment ) );
    }

    constexpr void deallocate( T * p, size_t )
    {
      global::free( p );
    }
  };
}  // namespace wabby::render::vulkan

#endif