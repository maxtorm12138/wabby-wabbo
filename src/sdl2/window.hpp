#ifndef _WAWY_SDL2_WINDOW_HPP
#define _WAWY_SDL2_WINDOW_HPP

// module
#include "util/noncopyable.hpp"

// sdl2
#include "SDL.h"
#include "SDL_vulkan.h"

// vulkan hpp
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_raii.hpp"

// std
#include "string"
#include "string_view"
#include "vector"

namespace wawy::sdl2
{

class window : public wawy::util::noncopyable
{
public:

    window(
        std::string_view title,
        int w,
        int h,
        int x = SDL_WINDOWPOS_UNDEFINED,
        int y = SDL_WINDOWPOS_UNDEFINED,
        uint32_t flags = SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);
    
    window(window &&other) noexcept;

    window &operator=(window &&other) noexcept;

    ~window();

public:
    void show();

    std::vector<std::string> get_vulkan_instance_extensions() const;

    vk::raii::SurfaceKHR create_vulkan_surface(const vk::raii::Instance &instance) const;

private:
    SDL_Window *window_{nullptr};
}; 
}

#endif