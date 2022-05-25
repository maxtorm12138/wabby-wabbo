#include "vk_globals.hpp"

namespace wabby::render::vulkan
{
  container::delayed<spdlog::logger> global::logger;
  void *                             global::allocator_user_args{ nullptr };
  pfn_allocation                     global::fn_allocation{ nullptr };
  pfn_reallocation                   global::fn_reallocation{ nullptr };
  pfn_free                           global::fn_free{ nullptr };
}  // namespace wabby::render::vulkan