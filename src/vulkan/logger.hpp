#ifndef _WABBY_VULKAN_LOGGER_HPP
#define _WABBY_VULKAN_LOGGER_HPP

// module
#include "util/log.hpp"
#include "util/api.hpp"

namespace wabby::vulkan
{

using namespace wabby::util::log_severity;

extern WABBY_API_HIDE wabby::util::logger g_logger;

}

#endif