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
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

// boost
#include "boost/dll/alias.hpp"
#include "boost/noncopyable.hpp"
#include "boost/predef.h"

// spdlog
#include "spdlog/spdlog.h"

// container
#include "container/registry.hpp"

extern std::shared_ptr<wabby::container::registry> g_registry;
namespace wabby::render::vulkan
{
  inline std::shared_ptr<spdlog::logger> logger( const std::string & name )
  {
    return g_registry->get<spdlog::logger>( name );
  }

}  // namespace wabby::render::vulkan

#endif