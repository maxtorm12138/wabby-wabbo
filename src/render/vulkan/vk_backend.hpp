#ifndef _WABBY_VULKAN_RENDERER_HPP
#define _WABBY_VULKAN_RENDERER_HPP

// module
#include "backend.hpp"
#include "vk_defines.hpp"
#include "vk_environment.hpp"
#include "vk_hardware.hpp"
#include "vk_device_allocator.hpp"

// std
#include "string"
#include "vector"
#include "functional"

// vulkan
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"


namespace wabby::render::vulkan
{
class vk_backend : public wabby::render::backend
{
public:
    vk_backend(const vk_backend_create_info &create_info);

public:
    void begin_frame() override;

    void end_frame() override;

    void begin_render_pass() override;

    void end_render_pass() override;

private:
    vk_environment environment_;
    vk::raii::SurfaceKHR surface_;
    vk_hardware hardware_;
    vk_device_allocator device_allocator_;
};
}

#endif