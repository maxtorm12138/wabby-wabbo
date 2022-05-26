#ifndef WABBY_RENDER_VULKAN_COMM_INCLUDE_HPP
#define WABBY_RENDER_VULKAN_COMM_INCLUDE_HPP

// std
#include "filesystem"
#include "fstream"
#include "functional"
#include "memory"
#include "optional"

// vulkan
#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-copy"
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"
#ifdef __GNUC__
#  pragma GCC diagnostic pop
#endif

// boost
#include "boost/algorithm/string.hpp"
#include "boost/noncopyable.hpp"
#include "boost/predef.h"

// spdlog
#include "spdlog/spdlog.h"

// container
#include "wabby/container/delayed.hpp"

// globals
#include "vk_container_warpper.hpp"
#include "vk_globals.hpp"

// fmt
#include "fmt/format.h"

#endif