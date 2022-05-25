#ifndef WABBY_RENDER_VULKAN_RENDERER_HPP
#define WABBY_RENDER_VULKAN_RENDERER_HPP

// backend interface
#include "wabby/render/backend.hpp"

extern "C"
{
  int  backend_setup( backend handle, const void * setup_info );
  int  backend_begin_frame( backend handle );
  int  backend_end_frame( backend handle );
  int  backend_begin_render_pass( backend handle );
  int  backend_end_render_pass( backend handle );
  int  backend_resized( backend handle );
  int  create_backend( backend * handle );
  void destroy_backend( backend backend );
  void set_backend_allocator( backend_allocator allocator );
}

#endif