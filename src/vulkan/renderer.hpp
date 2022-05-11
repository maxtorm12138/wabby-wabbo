#ifndef _WAWY_VULKAN_RENDERER_HPP
#define _WAWY_VULKAN_RENDERER_HPP

// module
#include "util/noncopyable.hpp"
#include "util/api.hpp"
#include "defines.hpp"

#define WAWY_API_IMPORT
#include "sdl2/sdl2.hpp"
#undef WAWY_API_IMPORT


namespace wawy::vulkan
{
class renderer_impl;
class WAWY_API_EXPORT renderer : public wawy::util::noncopyable
{
public:
    renderer(const wawy::sdl2::window &window, std::string_view application_name, uint32_t application_version);
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