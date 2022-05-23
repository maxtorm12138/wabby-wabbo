#include "vk_allocator.hpp"

namespace wabby::render::vulkan::detail
{
  void vk_allocator_impl::setup( void * user_args, fn_allocation fn_alloc, fn_reallocation fn_realloc, fn_free fn_fr ) noexcept
  {
    user_args_       = user_args;
    fn_allocation_   = fn_alloc;
    fn_reallocation_ = fn_realloc;
    fn_free_         = fn_fr;
  }
}  // namespace wabby::render::vulkan::detail