#ifndef _WABBY_DLL_DLL_POSIX_HPP
#define _WABBY_DLL_DLL_POSIX_HPP

// std
#include "string_view"
#include "filesystem"

// boost
#include "boost/noncopyable.hpp"


namespace wabby::dll
{

class dll_impl : public boost::noncopyable
{
protected:
    using native_handle_t = void *;

protected:
    void load(const std::filesystem::path &path);

    void unload();

    void *symbol_address(std::string_view symbol) noexcept;

protected:
    native_handle_t native_handle_{nullptr};
};

}

#endif