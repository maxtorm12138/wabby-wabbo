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
  struct vk_backend_context
  {
    vk_backend_context( const vk_backend_setup_info * create_info );

    vk_environment                       environment_;
    vk::raii::SurfaceKHR                 surface_;
    vk_hardware                          hardware_;
    vk_device_allocator                  device_allocator_;
    vk_swapchain                         swapchain_;
    vk_render_pass                       render_pass_;
    vk_framebuffers                      framebuffers_;
    uint32_t                             image_index_;
    uint64_t                             frame_index_;
    std::vector<vk::raii::CommandBuffer> command_buffers_;
    std::vector<vk::raii::Semaphore>     image_available_semaphores_;
    std::vector<vk::raii::Semaphore>     render_finished_semaphores_;
    std::vector<vk::raii::Fence>         in_flight_fences_;
  };

  class vk_backend : public boost::noncopyable
  {
  public:
    void setup( const vk_backend_setup_info * setup_info );

    void begin_frame();

    void end_frame();

    void begin_render_pass();

    void end_render_pass();

    void resized();

    void teardown();

  private:
    vk_backend_context * ctx_{ nullptr };
  };
}  // namespace wabby::render::vulkan

#endif