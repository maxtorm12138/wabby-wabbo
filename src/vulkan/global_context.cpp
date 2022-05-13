#include "global_context.hpp"

namespace wabby::vulkan
{
global_context &global_context::get()
{
    static global_context g_ctx;
    return g_ctx;
}

}