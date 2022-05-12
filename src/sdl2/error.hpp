#ifndef _WAWY_SDL2_ERROR_HPP
#define _WAWY_SDL2_ERROR_HPP

// moduel
#include "util/api.hpp"

// std
#include "stdexcept"
#include "string_view"


namespace wabby::sdl2
{

class WAWY_API_EXPORT sdl2_error : public std::runtime_error
{
public:
    sdl2_error(std::string_view sdl2_function_name);
};
}

#define SDL2_ASSERT(LOGIC, FUNCTION)\
    if (!(LOGIC)) throw sdl2_error(#FUNCTION)

#endif