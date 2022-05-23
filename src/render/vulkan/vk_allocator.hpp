#ifndef WABBY_RENDER_VULKAN_ALLOCATOR_HPP
#define WABBY_RENDER_VULKAN_ALLOCATOR_HPP

// std
#include "bit"

namespace wabby::render::vulkan
{
  namespace detail
  {
    class vk_allocator_impl
    {
    public:
      using fn_allocation   = void * (*)( void * user_args, size_t size, size_t alignment );
      using fn_reallocation = void * (*)( void * user_args, void * original, size_t size, size_t alignment );
      using fn_free         = void * (*)( void * user_args, void * memory );

    public:
      static void setup( void * user_args, fn_allocation fn_alloc, fn_reallocation fn_realloc, fn_free fn_fr ) noexcept;

    protected:
      static void *          user_args_;
      static fn_allocation   fn_allocation_;
      static fn_reallocation fn_reallocation_;
      static fn_free         fn_free_;
    };
  }  // namespace detail

  template <typename T>
  class vk_allocator : public detail::vk_allocator_impl
  {
  public:
    constexpr vk_allocator() noexcept {}

    constexpr vk_allocator( const vk_allocator & other ) noexcept {}

    template <class U>
    constexpr vk_allocator( const vk_allocator<U> & other ) noexcept
    {
    }

  public:
    [[nodiscard]] constexpr T * allocate( size_t n )
    {
      return static_cast<T *>( fn_allocation_( user_args_, sizeof( T ) * n, std::bit_ceil( sizeof( T ) ) ) );
    }

    constexpr void deallocate( T * p, size_t n )
    {
      fn_free_( user_args_, p );
    }

  private:
  };
}  // namespace wabby::render::vulkan

#endif