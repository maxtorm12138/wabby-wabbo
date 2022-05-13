#ifndef _WABBY_SDL2_CONTEXT_HPP
#define _WABBY_SDL2_CONTEXT_HPP

// module
#include "util/noncopyable.hpp"
#include "util/api.hpp"

// SDL
#include "SDL.h"

// std
#include "optional"

namespace wabby::sdl2
{

class WABBY_API_EXPORT context : public wabby::util::noncopyable
{
public:
    context(uint32_t flags = SDL_INIT_EVERYTHING);
    ~context();

public:
    std::optional<SDL_Event> poll_event();

    SDL_Event wait_event();

    void set_relative_mouse_mode(bool enabled);
};

}

#endif