#pragma once

// module
#include "util/noncopyable.hpp"

// sdl2
#include "SDL.h"
#include "SDL_vulkan.h"

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
    
    window(window &&ohter) noexcept;

    window &operator=(window &&other) noexcept;

    ~window();

public:
    void show();
    std::vector<std::string> get_vulkan_instance_extensions() const;
private:
    SDL_Window *window_{nullptr};
}; 
}