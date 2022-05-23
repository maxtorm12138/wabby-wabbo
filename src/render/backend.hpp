#ifndef _WABBY_RENDERER_RENDERER_HPP
#define _WABBY_RENDERER_RENDERER_HPP

// boost
#include "boost/config.hpp"

extern "C"
{
  typedef struct VkSurfaceKHR_T * VkSurfaceKHR;
  typedef struct VkInstance_T *   VkInstance;

  typedef struct
  {
    const char * application_name;
    uint32_t     appolcation_version;
  } backend_setup_info;

  typedef struct BOOST_SYMBOL_EXPORT
  {
    const char * application_name;
    uint32_t     appolcation_version;

    const char ** windowsystem_extensions;
    uint32_t      windowsystem_extensions_count;

    void * fn_make_surface_user_args;
    VkSurfaceKHR ( *fn_make_surface )( VkInstance, void * );

    void * fn_get_window_size_user_args;
    void ( *fn_get_window_size )( uint32_t *, uint32_t *, void * );

  } vk_backend_setup_info;

  typedef struct BOOST_SYMBOL_EXPORT
  {
    void * interal_handle;
  } backend_handle;

  typedef struct BOOST_SYMBOL_EXPORT
  {
    void ( *setup )( backend_handle * handle, const backend_setup_info * setup_info );

    void ( *begin_frame )( backend_handle * handle );

    void ( *end_frame )( backend_handle * handle );

    void ( *begin_render_pass )( backend_handle * handle );

    void ( *end_render_pass )( backend_handle * handle );

    void ( *resized )( backend_handle * handle );

    void ( *teardown )( backend_handle * handle );

  } backend_interface;

  BOOST_SYMBOL_EXPORT backend_handle * create_vk_backend();

  BOOST_SYMBOL_EXPORT void destroy_vk_backend();
}

#endif
