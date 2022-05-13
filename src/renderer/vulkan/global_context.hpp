#ifndef _WABBY_VULKAN_GLOBAL_CONTEXT_HPP
#define _WABBY_VULKAN_GLOBAL_CONTEXT_HPP

// module
#include "util/log.hpp"
#include "util/noncopyable.hpp"

namespace wabby::vulkan
{

using namespace wabby::util::log_severity;
class global_context : public wabby::util::noncopyable
{
public:
    static global_context &get();

    wabby::util::logger logger;
};

#define G_LOGGER global_context::get().logger

}

#endif