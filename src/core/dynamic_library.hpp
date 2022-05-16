#ifndef _WABBY_CORE_DYNAMIC_LIB_HPP
#define _WABBY_CORE_DYNAMIC_LIB_HPP

// boost
#include "boost/noncopyable.hpp"
#include "boost/predef.h"

// std
#include "filesystem"
#include "concepts"

// platform specific
#if BOOST_OS_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include "windows.h"
#else
    #include "dlfcn.h"
#endif

namespace wabby::core
{

class dynamic_library : public boost::noncopyable
{
public:

#if BOOST_OS_WINDOWS
    using native_handle_t = HINSTANCE;
#else
    using native_handle_t = void *;
#endif

    dynamic_library(std::filesystem::path library_path);

    ~dynamic_library() noexcept;

public:

    bool has(std::string_view symbol);

    native_handle_t native_handle() { return native_handle_; }

private:
    native_handle_t native_handle_{nullptr};
};

dynamic_library search_from_path(std::string_view library_name);

}

#endif