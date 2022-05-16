#ifndef _WABBY_SDL2_CONTEXT_HPP
#define _WABBY_SDL2_CONTEXT_HPP

// SDL
#include "SDL.h"

// boost
#include "boost/config.hpp"
#include "boost/noncopyable.hpp"

// std
#include "optional"

namespace wabby::sdl2
{

class BOOST_SYMBOL_EXPORT context : public boost::noncopyable
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