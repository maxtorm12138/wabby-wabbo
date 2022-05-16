#include "dynamic_library.hpp"

namespace wabby::core
{

dynamic_library::dynamic_library(std::filesystem::path library_path)
{
#if BOOST_OS_WINDOWS
    native_handle_ = LoadLibrary(library_path.string().c_str());
#else
    native_handle_ = dlopen(path, RTLD_NOW | RTLD_LOCAL);
#endif
}

dynamic_library::~dynamic_library() noexcept
{
    if (native_handle_ != nullptr)
    {
#if BOOST_OS_WINDOWS
        FreeLibrary(native_handle_);
#else
        dlclose(native_handle_);
#endif
    }

}


}