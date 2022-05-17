#ifndef _WABBY_DLL_DLL_HPP
#define _WABBY_DLL_DLL_HPP

// boost
#include "boost/predef.h"

// std
#include "filesystem"

// platform specific
#if BOOST_OS_WINDOWS
    #include "dll_windows.hpp"
    #define WIN32_LEAN_AND_MEAN
    #include "windows.h"
#else
    #include "dll_posix.hpp"
#endif

namespace wabby::dll
{

class dynamic_library : private dll_impl
{
public:
    using native_handle_t = dll_impl::native_handle_t;

public:
    dynamic_library(std::filesystem::path library_path);

    ~dynamic_library() noexcept;

public:
    bool has(std::string_view symbol) const noexcept;

    native_handle_t native_handle() noexcept { return native_handle_; }
};

dynamic_library search_from_path(std::string_view library_name);

}

#endif