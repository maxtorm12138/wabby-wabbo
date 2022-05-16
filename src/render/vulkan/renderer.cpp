#include "renderer.hpp"

// module
#include "global_context.hpp"
#include "environment.hpp"
#include "hardware.hpp"
#include "device_allocator.hpp"

// vulkan
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace wabby::vulkan
{

vk::ApplicationInfo build_application_info(const renderer::create_info &create_info)
{
    return vk::ApplicationInfo
    {
        .pApplicationName = create_info.applicaiton_name.c_str(),
        .applicationVersion = create_info.application_version,
        .pEngineName = WABBY_ENGINE_NAME.data(),
        .engineVersion = WABBY_ENGINE_VERSION,
        .apiVersion = VK_API_VERSION_1_1
    };
}

global_context_initializer initialize_global_context()
{
    {
        wabby::util::logger logger;
        logger.add_console_sink(std::cout);
        logger.add_file_sink("vulkan.log");
        G_LOGGER = std::move(logger);
    }

    return global_context_initializer{};
}

class renderer_impl : public wabby::util::noncopyable
{
public:
    renderer_impl(const renderer::create_info &create_info);

public:
    void begin_frame();
    void end_frame();
    void begin_render_pass();
    void end_render_pass();

private:
    vulkan::environment environment_;
    vk::raii::SurfaceKHR surface_;
    vulkan::hardware hardware_;
    vulkan::device_allocator allocator_;

    std::vector<vk::raii::Semaphore> image_available_semaphores_;
    std::vector<vk::raii::Semaphore> render_finishend_semaphores_;
    std::vector<vk::raii::Fence> in_flight_fences_;

    uint64_t frame_counter;
};

renderer::renderer(const create_info &create_info) :
    global_context_initializer_(initialize_global_context()),
    impl_(new renderer_impl(create_info))
{}

renderer::~renderer()
{
    delete impl_;
}

void renderer::begin_frame()
{
    return impl_->begin_frame();
}
void renderer::end_frame()
{
    return impl_->end_frame();
}

void renderer::begin_render_pass()
{
    return impl_->begin_render_pass();
}

void renderer::end_render_pass()
{
    return impl_->end_render_pass();
}

renderer_impl::renderer_impl(const renderer::create_info &create_info) :
    environment_(build_application_info(create_info), create_info.windowsystem_extensions),
    surface_(environment_.instance(), create_info.fn_make_surface(*environment_.instance())),
    hardware_(environment_.instance(), surface_),
    allocator_(environment_.instance(), hardware_.physical_device(), hardware_.device())
{
}

void renderer_impl::begin_frame()
{

}

void renderer_impl::end_frame()
{

}

void renderer_impl::begin_render_pass()
{

}

void renderer_impl::end_render_pass()
{

}

}