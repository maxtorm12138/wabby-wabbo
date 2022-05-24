#ifndef WABBY_RENDERER_RENDERER_HPP
#define WABBY_RENDERER_RENDERER_HPP

// boost
#include "boost/config.hpp"

// std
#include "cstddef"
#include "cstdint"

// avoid include vulkan.h
extern "C"
{
  typedef struct VkSurfaceKHR_T * VkSurfaceKHR;
  typedef struct VkInstance_T *   VkInstance;
}

extern "C"
{
  typedef struct
  {
    const char * application_name;
    uint32_t     application_version;
  } backend_setup_info;

  typedef struct
  {
    const char * application_name;
    uint32_t     application_version;

    const char ** windowsystem_extensions;
    uint32_t      windowsystem_extensions_count;

    void * fn_make_surface_user_args;
    VkSurfaceKHR ( *fn_make_surface )( void *, VkInstance );

    void * fn_get_window_size_user_args;
    void ( *fn_get_window_size )( void *, uint32_t *, uint32_t * );

    const char * configuration_path;
  } vk_backend_setup_info;

  typedef struct backend_t
  {
    void * internal_handle;

    void ( *setup )( backend_t * handle, const backend_setup_info * setup_info );

    void ( *teardown )( backend_t * handle );

    void ( *begin_frame )( backend_t * handle );

    void ( *end_frame )( backend_t * handle );

    void ( *begin_render_pass )( backend_t * handle );

    void ( *end_render_pass )( backend_t * handle );

    void ( *resized )( backend_t * handle );

  } * backend;

  typedef void * ( *fn_allocation )( void * user_args, size_t size, size_t alignment );
  typedef void * ( *fn_reallocation )( void * user_args, void * original, size_t size, size_t alignment );
  typedef void ( *fn_free )( void * user_args, void * memory );

  typedef struct allocation_callbacks_t
  {
    void *          user_args;
    fn_allocation   allocation;
    fn_reallocation reallocation;
    fn_free         free;
  } allocation_callbacks;

  extern BOOST_SYMBOL_EXPORT void set_allocation_callbacks( const allocation_callbacks * allocation_callbacks );
  extern BOOST_SYMBOL_EXPORT int  create_backend( backend * backend );
  extern BOOST_SYMBOL_EXPORT void destroy_backend( backend backend );
}

#endif
