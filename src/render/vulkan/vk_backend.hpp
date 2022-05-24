#ifndef WABBY_RENDER_VULKAN_RENDERER_HPP
#define WABBY_RENDER_VULKAN_RENDERER_HPP

// backend interface
#include "wabby/render/backend.hpp"

// module
#include "vk_allocator.hpp"
#include "vk_defines.hpp"
#include "vk_device_allocator.hpp"
#include "vk_environment.hpp"
#include "vk_framebuffers.hpp"
#include "vk_hardware.hpp"
#include "vk_render_pass.hpp"
#include "vk_swapchian.hpp"

// common include
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{

  class vk_backend : public boost::noncopyable
  {
  public:
    vk_backend() = default;

    void setup( const vk_backend_setup_info * setup_info );

    void begin_frame();

    void end_frame();

    void begin_render_pass();

    void end_render_pass();

    void resized();

    void teardown();

  private:
    wabby::container::delayed<vk_environment>       environment_;
    wabby::container::delayed<vk::raii::SurfaceKHR> surface_;
    wabby::container::delayed<vk_hardware>          hardware_;
    wabby::container::delayed<vk_device_allocator>  device_allocator_;
    wabby::container::delayed<vk_swapchain>         swapchain_;
    wabby::container::delayed<vk_render_pass>       render_pass_;
    wabby::container::delayed<vk_framebuffers>      framebuffers_;
    uint32_t                                        image_index_;
    uint64_t                                        frame_index_;
    std::vector<vk::raii::CommandBuffer>            command_buffers_;
    std::vector<vk::raii::Semaphore>                image_available_semaphores_;
    std::vector<vk::raii::Semaphore>                render_finished_semaphores_;
    std::vector<vk::raii::Fence>                    in_flight_fences_;
  };
}  // namespace wabby::render::vulkan

#endif