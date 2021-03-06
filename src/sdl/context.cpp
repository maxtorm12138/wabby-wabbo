#include "wabby/sdl2/context.hpp"

// module
#include "wabby/sdl2/error.hpp"

namespace wabby::sdl2
{

  context::context( uint32_t flags )
  {
    int ret = SDL_Init( flags );
    SDL2_ASSERT( ret == 0, SDL_Init );
  }

  context::~context()
  {
    SDL_Quit();
  }

  std::optional<SDL_Event> context::poll_event() noexcept
  {
    SDL_Event event;
    int       ret = SDL_PollEvent( &event );
    if ( ret == 0 )
    {
      return {};
    }

    return event;
  }

  SDL_Event context::wait_event()
  {
    SDL_Event event;
    int       ret = SDL_WaitEvent( &event );
    SDL2_ASSERT( ret == 1, SDL_WaitEvent );

    return event;
  }

  void context::set_relative_mouse_mode( bool enabled )
  {
    int ret = SDL_SetRelativeMouseMode( enabled ? SDL_TRUE : SDL_FALSE );
    SDL2_ASSERT( ret == 0, SDL_SetRelativeMouseMode );
  }
}  // namespace wabby::sdl2