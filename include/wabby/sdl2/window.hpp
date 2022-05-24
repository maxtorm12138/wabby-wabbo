#ifndef _WABBY_SDL2_WINDOW_HPP
#define _WABBY_SDL2_WINDOW_HPP

// boost
#include "boost/config.hpp"
#include "boost/noncopyable.hpp"

// sdl2
#include "SDL.h"
#include "SDL_vulkan.h"

// std
#include "string"
#include "string_view"
#include "vector"

namespace wabby::sdl2
{

  class window : public boost::noncopyable
  {
  public:
    window( std::string_view title,
            int              w,
            int              h,
            int              x     = SDL_WINDOWPOS_UNDEFINED,
            int              y     = SDL_WINDOWPOS_UNDEFINED,
            uint32_t         flags = SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI );

    window( window && other ) noexcept;

    window & operator=( window && other ) noexcept;

    ~window();

  public:
    void show() noexcept;

    std::vector<std::string> get_vulkan_instance_extensions() const;

    VkSurfaceKHR create_vulkan_surface( VkInstance instance ) const;

    std::pair<uint32_t, uint32_t> get_vulakn_drawable_size() const;

    void set_resizeable( bool resizeable ) const;

  private:
    SDL_Window * window_{ nullptr };
  };
}  // namespace wabby::sdl2

#endif