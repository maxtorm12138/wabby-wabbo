#include "renderer.hpp"

// module
#include "instance.hpp"

// vulkan
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

namespace wawy::vulkan
{

class renderer_impl : public wawy::util::noncopyable
{
public:
    renderer_impl();

public:
    void begin_frame();
    void end_frame();
    void begin_render_pass();
    void end_render_pass();

private:
    ::vk::raii::Context context_;
    instance instance_;
};


renderer::renderer() :
    impl_(new renderer_impl)
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

}