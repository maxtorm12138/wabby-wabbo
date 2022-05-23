#ifndef _WABBY_VULKAN_COMM_INCLUDE_HPP
#define _WABBY_VULKAN_COMM_INCLUDE_HPP

// std
#include "functional"
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

// container
#include "container/registry.hpp"

extern std::shared_ptr<wabby::container::registry> g_registry;
#define LOGGER( NAME ) ( g_registry->get<spdlog::logger>( NAME ) )

#endif