#ifndef _WABBY_VULKAN_RENDERER_HPP
#define _WABBY_VULKAN_RENDERER_HPP

// module
#include "backend.hpp"
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
    vk_backend_context( const vk_backend_create_info & create_info );

    vk_environment                       environment_;
    vk::raii::SurfaceKHR                 surface_;
    vk_hardware                          hardware_;
    vk_device_allocator                  device_allocator_;
    vk_swapchain                         swapchain_;
    vk_render_pass                       render_pass_;
    vk_framebuffers                      framebuffers_;
    int64_t                              image_index_;
    uint64_t                             frame_index_;
    std::vector<vk::raii::CommandBuffer> command_buffers_;
    std::vector<vk::raii::Semaphore>     image_available_semaphores_;
    std::vector<vk::raii::Semaphore>     render_finished_semaphores_;
    std::vector<vk::raii::Fence>         in_flight_fences_;
  };

  class vk_backend : public wabby::render::backend
  {
  public:
    void setup( const backend_create_info & create_info ) override;

    void begin_frame() override;

    void end_frame() override;

    void begin_render_pass() override;

    void end_render_pass() override;

    void resized() override;

    void teardown() override;

  private:
    std::vector<std::shared_ptr<spdlog::logger>> loggers_;
    std::optional<vk_backend_context>            ctx_;
  };
}  // namespace wabby::render::vulkan

#endif