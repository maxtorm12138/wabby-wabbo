#ifndef _WABBY_VULKAN_RENDERER_HPP
#define _WABBY_VULKAN_RENDERER_HPP

// module
#include "util/noncopyable.hpp"
#include "util/api.hpp"
#include "defines.hpp"

// std
#include "string"
#include "vector"
#include "functional"

extern "C"
{
typedef struct VkSurfaceKHR_T *VkSurfaceKHR;
typedef struct VkInstance_T *VkInstance;
}

namespace wabby::vulkan
{
class renderer_impl;

class global_context_initializer
{};

class WABBY_API_EXPORT renderer : public wabby::util::noncopyable
{
public:

    struct create_info
    {
        std::string applicaiton_name;
        uint32_t application_version;
        std::vector<std::string> windowsystem_extensions;
        std::function<VkSurfaceKHR(VkInstance)> fn_make_surface;
        std::function<std::pair<uint32_t, uint32_t>()> fn_get_window_size;
    };

    renderer(const create_info &create_info);
    ~renderer();

public:
    void begin_frame();

    void end_frame();

    void begin_render_pass();

    void end_render_pass();

private:
    global_context_initializer global_context_initializer_;
    renderer_impl *impl_;
};
}

#endif