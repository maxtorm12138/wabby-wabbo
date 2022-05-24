#ifndef WABBY_RENDER_BACKEND_HPP
#define WABBY_RENDER_BACKEND_HPP
// module
#include "backend.hpp"

// boost
#include "boost/dll/runtime_symbol_info.hpp"
#include "boost/dll/shared_library.hpp"

namespace wabby::render::raii
{
  class backend
  {
  public:
    backend( const boost::dll::fs::path & path, const allocation_callbacks_t * allocation_callbacks )
    {
      library_.load( path );

      auto fn_set_allocation_callbacks = library_.get<decltype( ::set_allocation_callbacks )>( "set_allocation_callbacks" );

      auto create_backend = library_.get<decltype( ::create_backend )>( "create_backend" );

      fn_set_allocation_callbacks( allocation_callbacks );

      create_backend( &backend_handle_ );
    }

    ~backend()
    {
      if ( backend_handle_ != nullptr )
      {
        destroy_backend( backend_handle_ );
      }
    }

  public:
    void setup( const backend_setup_info * setup_info )
    {
      backend_handle_->setup( backend_handle_, setup_info );
    }

    void teardown()
    {
      backend_handle_->teardown( backend_handle_ );
    }

    void begin_frame()
    {
      backend_handle_->begin_frame( backend_handle_ );
    }

    void end_frame()
    {
      backend_handle_->end_frame( backend_handle_ );
    }

    void begin_render_pass()
    {
      backend_handle_->begin_render_pass( backend_handle_ );
    }

    void end_render_pass()
    {
      backend_handle_->end_render_pass( backend_handle_ );
    }

    void resized()
    {
      backend_handle_->resized( backend_handle_ );
    }

  private:
    boost::dll::shared_library library_{};
    ::backend                  backend_handle_{ nullptr };
  };
}  // namespace wabby::render::raii
#endif