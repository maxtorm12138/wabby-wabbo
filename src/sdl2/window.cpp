#include "window.hpp"

// module
#include "error.hpp"

namespace wawy::sdl2
{

window::window(
    std::string_view title,
    int w,
    int h,
    int x,
    int y,
    uint32_t flags) :
    window_()
{
    window_ = SDL_CreateWindow(title.data(), x, y, w, h, flags);
    SDL2_ASSERT(window_ != nullptr, SDL_CreateWindow);
}

}