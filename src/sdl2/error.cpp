#include "error.hpp"

//fmt
#include "fmt/format.h"

// sdl2
#include "SDL.h"

namespace wawy::sdl2
{

sdl2_error::sdl2_error(std::string_view sdl2_function_name) :
    std::runtime_error::runtime_error(fmt::format("calling {} fail, error: {}", sdl2_function_name, SDL_GetError()))
{
}

}