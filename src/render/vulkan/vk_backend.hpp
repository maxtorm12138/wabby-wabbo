// backend interface
#include "wabby/render/backend.hpp"

// module
#include "vk_allocator.hpp"
#include "vk_command_buffers.hpp"
#include "vk_command_pool.hpp"
#include "vk_defines.hpp"
#include "vk_device_allocator.hpp"
#include "vk_environment.hpp"
#include "vk_fences.hpp"
#include "vk_framebuffers.hpp"
#include "vk_hardware.hpp"
#include "vk_render_pass.hpp"
#include "vk_semaphores.hpp"
#include "vk_surface.hpp"
#include "vk_swapchian.hpp"

// comm include
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{
  class vk_backend : public boost::noncopyable
  {
  public:
    vk_backend() {}

    ~vk_backend() {}

    int setup( const vk_backend_setup_info * setup_info );

    int begin_frame();

    int end_frame();

    int begin_render_pass();

    int end_render_pass();

    int resized();

    int teardown();

  private:
    container::delayed<vk_environment>      environment_;
    container::delayed<vk_surface>          surface_;
    container::delayed<vk_hardware>         hardware_;
    container::delayed<vk_queue_cache>      queue_cache_;
    container::delayed<vk_command_pool>     graphics_command_pool_;
    container::delayed<vk_device_allocator> device_allocator_;
    container::delayed<vk_swapchain>        swapchain_;
    container::delayed<vk_render_pass>      render_pass_;
    container::delayed<vk_framebuffers>     framebuffers_;
    uint32_t                                image_index_;
    uint64_t                                frame_index_;
    container::delayed<vk_command_buffers>  command_buffers_;
    container::delayed<vk_semaphores>       image_available_semaphores_;
    container::delayed<vk_semaphores>       render_finished_semaphores_;
    container::delayed<vk_fences>           in_flight_fences_;
  };
}  // namespace wabby::render::vulkan
