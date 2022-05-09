#ifndef _WAWY_SDL2_CONTEXT_HPP
#define _WAWY_SDL2_CONTEXT_HPP

// module
#include "util/noncopyable.hpp"

// SDL
#include "SDL.h"

// std
#include "optional"

// boost
#include "boost/dll.hpp"

namespace wawy::sdl2
{

class BOOST_SYMBOL_EXPORT context : public wawy::util::noncopyable
{
public:
    context(uint32_t flags = SDL_INIT_EVERYTHING);
    ~context();

public:
    std::optional<SDL_Event> poll_event();
    SDL_Event wait_event();
};

}

#endif