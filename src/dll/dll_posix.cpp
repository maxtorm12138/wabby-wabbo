#include "dll_posix.hpp"

// platform specific
#include "dlfcn.h"

namespace wabby::dll
{

void dll_impl::load(const std::filesystem::path &path)
{
}

void dll_impl::unload()
{

}

void *dll_impl::symbol_address(std::string_view symbol) noexcept
{
    auto sym = dlsym(native_handle_, symbol.data());
    if (sym == nullptr)
    {
    }
    return nullptr;
}

}