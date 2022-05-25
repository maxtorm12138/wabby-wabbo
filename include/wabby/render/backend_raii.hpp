#ifndef WABBY_RENDER_BACKEND_HPP
#define WABBY_RENDER_BACKEND_HPP
// module
#include "backend.hpp"

// boost
#include "boost/dll/library_info.hpp"
#include "boost/dll/runtime_symbol_info.hpp"
#include "boost/dll/shared_library.hpp"

namespace wabby::render::raii
{
  class backend
  {
  public:
    backend( const boost::dll::fs::path & path, backend_allocator allocation_callbacks )
    {
      library_.load( path );
      boost::dll::library_info info( path );
      for ( auto sym : info.symbols() )
      {
        printf( "%s\n", sym.c_str() );
      }
    }

    ~backend() {}

  public:
    void setup( const void * setup_info ) {}

    void teardown() {}

    void begin_frame() {}

    void end_frame() {}

    void begin_render_pass() {}

    void end_render_pass() {}

    void resized() {}

  private:
    boost::dll::shared_library library_{};
    ::backend                  backend_handle_{ nullptr };
  };
}  // namespace wabby::render::raii
#endif