#ifndef WABBY_RENDERER_RENDERER_HPP
#define WABBY_RENDERER_RENDERER_HPP

#include "wabby/api.hpp"

// std
#include "cstddef"
#include "cstdint"

// avoid include vulkan.h
#ifdef __cplusplus
extern "C"
{
#endif
  typedef struct VkSurfaceKHR_T * VkSurfaceKHR;
  typedef struct VkInstance_T *   VkInstance;
#ifdef __cplusplus
}
#endif  // DEBUG

#ifdef __cplusplus
extern "C"
{
#endif

  typedef void *                       backend;
  typedef struct backend_allocator_t * backend_allocator;

  typedef VkSurfaceKHR ( *pfn_vk_create_surface )( void * user_args, VkInstance instance );
  typedef void ( *pfn_get_window_size )( void * user_args, uint32_t * width, uint32_t * height );

  typedef void * ( *pfn_allocation )( void * user_args, size_t size, size_t alignment );
  typedef void * ( *pfn_reallocation )( void * user_args, void * original, size_t size, size_t alignment );
  typedef void ( *pfn_free )( void * user_args, void * memory );

  typedef int ( *pfn_backend_setup )( backend handle, const void * setup_info );
  typedef int ( *pfn_backend_teardown )( backend handle );
  typedef int ( *pfn_backend_begin_frame )( backend handle );
  typedef int ( *pfn_backend_end_frame )( backend handle );
  typedef int ( *pfn_backend_begin_render_pass )( backend handle );
  typedef int ( *pfn_backend_end_render_pass )( backend handle );
  typedef int ( *pfn_backend_resized )( backend handle );

  typedef int ( *pfn_create_backend )( backend * p_backend );
  typedef void ( *pfn_destroy_backend )( backend backend );

  typedef void ( *pfn_set_backend_allocator )( backend_allocator allocator );

  typedef struct backend_allocator_t
  {
    void *           user_args;
    pfn_allocation   fn_allocation;
    pfn_reallocation fn_reallocation;
    pfn_free         fn_free;
  } backend_allocator_t;

  typedef struct
  {
    const char * application_name;
    uint32_t     application_version;

    const char * configuration_path;

    const char ** windowsystem_extensions;
    uint32_t      windowsystem_extensions_count;

    void *                user_args;
    pfn_vk_create_surface fn_vk_create_surface;
    pfn_get_window_size   fn_get_window_size;

  } vk_backend_setup_info;

  extern WABBY_API_PUBLIC void * get_proc_addr( const char * name );

#ifdef __cplusplus
}
#endif

#endif
