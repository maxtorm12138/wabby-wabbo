#ifndef _WAWY_SDL2_ERROR_HPP
#define _WAWY_SDL2_ERROR_HPP

// std
#include "stdexcept"
#include "string_view"

namespace wawy::sdl2
{

class sdl2_error : public std::runtime_error
{
public:
    sdl2_error(std::string_view sdl2_function_name);
};
}

#define SDL2_ASSERT(LOGIC, FUNCTION)\
    if (!(LOGIC)) throw sdl2_error(#FUNCTION)

#endif