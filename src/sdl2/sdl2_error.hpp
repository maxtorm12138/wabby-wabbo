#pragma once

// std
#include "stdexcept"

namespace wawy::sdl2
{

class sdl2_error : public std::runtime_error
{
public:
};
}