#ifndef _WABBY_SDL2_ERROR_HPP
#define _WABBY_SDL2_ERROR_HPP

// std
#include "stdexcept"
#include "string_view"

namespace wabby::sdl2
{
  class sdl2_error : public std::runtime_error
  {
  public:
    sdl2_error( std::string_view sdl2_function_name );
  };
}  // namespace wabby::sdl2

#define SDL2_ASSERT( LOGIC, FUNCTION ) \
  if ( !( LOGIC ) )                    \
  throw sdl2_error( #FUNCTION )

#endif