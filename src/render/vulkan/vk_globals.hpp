#ifndef WABBY_RENDER_VULKAN_GLOBALS_HPP
#define WABBY_RENDER_VULKAN_GLOBALS_HPP

// spdlog
#include "spdlog/spdlog.h"

// container
#include "wabby/container/delayed.hpp"

namespace wabby::render::vulkan
{
  typedef void * ( *pfn_allocation )( void * user_args, size_t size, size_t alignment );
  typedef void * ( *pfn_reallocation )( void * user_args, void * original, size_t size, size_t alignment );
  typedef void ( *pfn_free )( void * user_args, void * memory );

  class global
  {
  public:
    static container::delayed<spdlog::logger> logger;
    static void *                             allocator_user_args;
    static pfn_allocation                     fn_allocation;
    static pfn_reallocation                   fn_reallocation;
    static pfn_free                           fn_free;
  };
}  // namespace wabby::render::vulkan

#endif