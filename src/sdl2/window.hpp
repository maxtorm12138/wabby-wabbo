#ifndef _WAWY_SDL2_WINDOW_HPP
#define _WAWY_SDL2_WINDOW_HPP

// module
#include "util/noncopyable.hpp"

// sdl2
#include "SDL.h"
#include "SDL_vulkan.h"

// vulkan
#include "vulkan/vulkan.h"

// std
#include "string"
#include "string_view"
#include "vector"

// boost
#include "boost/dll.hpp"

namespace wawy::sdl2
{

class BOOST_SYMBOL_EXPORT window : public wawy::util::noncopyable
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

    VkSurfaceKHR create_vulkan_surface(VkInstance instance) const;

private:
    SDL_Window *window_{nullptr};
}; 
}

#endif