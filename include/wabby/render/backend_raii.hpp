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
    backend( const boost::dll::fs::path & path, backend_allocator allocator ) : library_( path )
    {
      get_proc_addr_             = library_.get<pfn_get_proc_addr>( "get_proc_addr" );
      backend_setup_             = reinterpret_cast<pfn_backend_setup>( get_proc_addr_( "backend_setup" ) );
      backend_teardown_          = reinterpret_cast<pfn_backend_teardown>( get_proc_addr_( "backend_teardown" ) );
      backend_begin_frame_       = reinterpret_cast<pfn_backend_begin_frame>( get_proc_addr_( "backend_begin_frame" ) );
      backend_end_frame_         = reinterpret_cast<pfn_backend_end_frame>( get_proc_addr_( "backend_end_frame" ) );
      backend_begin_render_pass_ = reinterpret_cast<pfn_backend_begin_render_pass>( get_proc_addr_( "backend_begin_render_pass" ) );
      backend_end_render_pass_   = reinterpret_cast<pfn_backend_end_render_pass>( get_proc_addr_( "backend_end_render_pass" ) );
      backend_resized_           = reinterpret_cast<pfn_backend_resized>( get_proc_addr_( "backend_resized" ) );
      create_backend_            = reinterpret_cast<pfn_create_backend>( get_proc_addr_( "create_backend" ) );
      destroy_backend_           = reinterpret_cast<pfn_destroy_backend>( get_proc_addr_( "destroy_backend" ) );

      auto set_backend_allocator = reinterpret_cast<pfn_set_backend_allocator>( get_proc_addr_( "set_backend_allocator" ) );
      set_backend_allocator( allocator );

      create_backend_( &native_handle_ );
    }

    ~backend()
    {
      destroy_backend_( native_handle_ );
    }

  public:
    void setup( const void * setup_info )
    {
      backend_setup_( native_handle_, setup_info );
    }

    void teardown()
    {
      backend_teardown_( native_handle_ );
    }

    void begin_frame()
    {
      backend_begin_frame_( native_handle_ );
    }

    void end_frame()
    {
      backend_end_frame_( native_handle_ );
    }

    void begin_render_pass()
    {
      backend_begin_render_pass_( native_handle_ );
    }

    void end_render_pass()
    {
      backend_end_render_pass_( native_handle_ );
    }

    void resized()
    {
      backend_resized_( native_handle_ );
    }

  private:
    boost::dll::shared_library    library_{};
    pfn_get_proc_addr             get_proc_addr_;
    pfn_create_backend            create_backend_;
    pfn_destroy_backend           destroy_backend_;
    pfn_backend_setup             backend_setup_;
    pfn_backend_teardown          backend_teardown_;
    pfn_backend_begin_frame       backend_begin_frame_;
    pfn_backend_end_frame         backend_end_frame_;
    pfn_backend_begin_render_pass backend_begin_render_pass_;
    pfn_backend_end_render_pass   backend_end_render_pass_;
    pfn_backend_resized           backend_resized_;
    ::backend                     native_handle_{ nullptr };
  };
}  // namespace wabby::render::raii
#endif