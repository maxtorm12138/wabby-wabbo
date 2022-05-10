#ifndef _WAWY_VULKAN_RENDERER_HPP
#define _WAWY_VULKAN_RENDERER_HPP

// module
#include "util/noncopyable.hpp"
#include "sdl2/sdl2.hpp"

// boost
#include "boost/dll.hpp"

namespace wawy::vulkan
{
class renderer_impl;
class BOOST_SYMBOL_EXPORT renderer : public wawy::util::noncopyable
{
public:
    renderer(const wawy::sdl2::window &window);
    ~renderer();

public:
    void begin_frame();
    void end_frame();
    void begin_render_pass();
    void end_render_pass();
private:
    renderer_impl *impl_;
};
}

#endif