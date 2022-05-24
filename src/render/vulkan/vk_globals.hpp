#ifndef WABBY_RENDER_VULKAN_GLOBALS_HPP
#define WABBY_RENDER_VULKAN_GLOBALS_HPP

// spdlog
#include "spdlog/spdlog.h"

// container
#include "wabby/container/delayed.hpp"

namespace wabby::render::vulkan
{
  class global
  {
  public:
    static container::delayed<spdlog::logger> logger;
  };
}  // namespace wabby::render::vulkan

#endif