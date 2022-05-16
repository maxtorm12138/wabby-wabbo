#ifndef _WABBY_VULKAN_RENDERER_HPP
#define _WABBY_VULKAN_RENDERER_HPP

// module
#include "vk_defines.hpp"
#include "backend.hpp"

// std
#include "string"
#include "vector"
#include "functional"

extern "C"
{
typedef struct VkSurfaceKHR_T *VkSurfaceKHR;
typedef struct VkInstance_T *VkInstance;
}

namespace wabby::render::vulkan
{

class vk_backend_impl;

class vk_backend : public wabby::render::backend
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

    vk_backend(const create_info &create_info);

public:
    void begin_frame() override;

    void end_frame() override;

    void begin_render_pass() override;

    void end_render_pass() override;

private:
    vk_backend_impl *impl_;
};

std::shared_ptr<wabby::render::backend> BOOST_SYMBOL_EXPORT make_vk_backend(const vk_backend::create_info &create_info);

}

#endif