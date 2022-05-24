#ifndef WABBY_RENDER_VULKAN_GLOBALS_HPP
#define WABBY_RENDER_VULKAN_GLOBALS_HPP

// spdlog
#include "spdlog/spdlog.h"

// container
#include "wabby/container/delayed.hpp"

namespace wabby::render::vulkan::global
{
  extern container::delayed<spdlog::logger> logger;
}
#endif