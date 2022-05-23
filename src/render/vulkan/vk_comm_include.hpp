#ifndef WABBY_RENDER_VULKAN_COMM_INCLUDE_HPP
#define WABBY_RENDER_VULKAN_COMM_INCLUDE_HPP

// std
#include "functional"
#include "memory"
#include "optional"
#include "string"
#include "unordered_map"
#include "unordered_set"
#include "vector"

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
#include "boost/dll/alias.hpp"
#include "boost/noncopyable.hpp"
#include "boost/predef.h"

// spdlog
#include "spdlog/spdlog.h"

namespace wabby::render::vulkan
{
}
#endif