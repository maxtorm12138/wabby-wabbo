// backend interface
#include "vk_backend.hpp"
#include "wabby/render/backend.hpp"


#define REGISTER_PROC_ADDR( PFN, FN )                 \
  {                                                   \
#    FN, reinterpret_cast < pfn_void_function>( &FN ) \
  }

extern "C"
{
  int backend_setup( backend handle, const void * setup_info )
  {
    return static_cast<wabby::render::vulkan::vk_backend *>( handle )->setup( reinterpret_cast<const vk_backend_setup_info *>( setup_info ) );
  }

  int backend_teardown( backend handle )
  {
    return static_cast<wabby::render::vulkan::vk_backend *>( handle )->teardown();
  }

  int backend_begin_frame( backend handle )
  {
    return static_cast<wabby::render::vulkan::vk_backend *>( handle )->begin_frame();
  }

  int backend_end_frame( backend handle )
  {
    return static_cast<wabby::render::vulkan::vk_backend *>( handle )->end_frame();
  }

  int backend_begin_render_pass( backend handle )
  {
    return static_cast<wabby::render::vulkan::vk_backend *>( handle )->begin_render_pass();
  }

  int backend_end_render_pass( backend handle )
  {
    return static_cast<wabby::render::vulkan::vk_backend *>( handle )->end_render_pass();
  }

  int backend_resized( backend handle )
  {
    return static_cast<wabby::render::vulkan::vk_backend *>( handle )->resized();
  }

  int create_backend( backend * p_handle )
  {
    using namespace wabby::render::vulkan;
    auto p_backend = static_cast<vk_backend *>( global::allocation( sizeof( vk_backend ), 0 ) );
    std::construct_at( p_backend );
    *p_handle = p_backend;
    return 0;
  }

  void destroy_backend( backend handle )
  {
    using namespace wabby::render::vulkan;
    auto p_backend = static_cast<vk_backend *>( handle );
    std::destroy_at( p_backend );
    global::free( p_backend );
  }

  void set_backend_allocator( backend_allocator allocator )
  {
    using namespace wabby::render::vulkan;
    global::allocation    = fn_allocation( allocator->fn_allocation, allocator->user_args );
    global::realllocation = fn_reallocation( allocator->fn_reallocation, allocator->user_args );
    global::free          = fn_free( allocator->fn_free, allocator->user_args );
  }

  pfn_void_function get_proc_addr( const char * name )
  {
    static std::pair<const char *, pfn_void_function> addresses[] = {
      REGISTER_PROC_ADDR( pfn_backend_setup, backend_setup ),
      REGISTER_PROC_ADDR( pfn_backend_teardown, backend_teardown ),
      REGISTER_PROC_ADDR( pfn_set_backend_allocator, set_backend_allocator ),
      REGISTER_PROC_ADDR( pfn_destroy_backend, destroy_backend ),
      REGISTER_PROC_ADDR( pfn_create_backend, create_backend ),
      REGISTER_PROC_ADDR( pfn_backend_resized, backend_resized ),
      REGISTER_PROC_ADDR( pfn_backend_end_render_pass, backend_end_render_pass ),
      REGISTER_PROC_ADDR( pfn_backend_begin_render_pass, backend_begin_render_pass ),
      REGISTER_PROC_ADDR( pfn_backend_end_frame, backend_end_frame ),
      REGISTER_PROC_ADDR( pfn_backend_begin_frame, backend_begin_frame ),
    };

    for ( size_t i = 0; i < sizeof( addresses ) / sizeof( std::pair<const char *, void *> ); i++ )
    {
      if ( strcmp( name, addresses[i].first ) == 0 )
      {
        return addresses[i].second;
      }
    }
    return nullptr;
  }
}