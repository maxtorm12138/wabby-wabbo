#ifndef WABBY_RENDER_VULKAN_GLOBALS_HPP
#define WABBY_RENDER_VULKAN_GLOBALS_HPP
// alloc function defs
#include "wabby/render/backend.hpp"

// spdlog
#include "spdlog/spdlog.h"

// container
#include "wabby/container/delayed.hpp"

namespace wabby::render::vulkan
{
  struct fn_allocation
  {
    fn_allocation( pfn_allocation allocation = nullptr, void * user_args = nullptr );

    void * operator()( size_t size, size_t alignment );

  private:
    void * user_args_;

    pfn_allocation allocation_;
  };

  struct fn_free
  {
    fn_free( pfn_free free = nullptr, void * user_args = nullptr );

    void operator()( void * memory );

  private:
    void * user_args_;

    pfn_free free_;
  };

  struct fn_reallocation
  {
    fn_reallocation( pfn_reallocation reallocation = nullptr, void * user_args = nullptr );

    void * operator()( void * original, size_t size, size_t alignment );

  private:
    void * user_args_;

    pfn_reallocation reallocation_;
  };

  class global
  {
  public:
    static container::delayed<spdlog::logger> logger;
    static fn_allocation                      allocation;
    static fn_reallocation                    realllocation;
    static fn_free                            free;
  };
}  // namespace wabby::render::vulkan

#endif