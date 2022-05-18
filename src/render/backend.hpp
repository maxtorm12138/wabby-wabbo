#ifndef _WABBY_RENDERER_RENDERER_HPP
#define _WABBY_RENDERER_RENDERER_HPP

// boost
#include "boost/config.hpp"
#include "boost/noncopyable.hpp"

// std
#include "functional"
#include "string"
#include "vector"

extern "C"
{
  typedef struct VkSurfaceKHR_T * VkSurfaceKHR;
  typedef struct VkInstance_T *   VkInstance;
}

namespace wabby::render
{
  class BOOST_SYMBOL_EXPORT backend : boost::noncopyable
  {
  public:
    backend()          = default;
    virtual ~backend() = default;

  public:
    virtual void begin_frame() = 0;

    virtual void end_frame() = 0;

    virtual void begin_render_pass() = 0;

    virtual void end_render_pass() = 0;

    virtual void resized() = 0;
  };

  struct BOOST_SYMBOL_EXPORT vk_backend_create_info
  {
    std::string                                    applicaiton_name;
    uint32_t                                       application_version;
    std::vector<std::string>                       windowsystem_extensions;
    std::function<VkSurfaceKHR( VkInstance )>      fn_make_surface;
    std::function<std::pair<uint32_t, uint32_t>()> fn_get_window_size;
  };

  BOOST_SYMBOL_EXPORT std::shared_ptr<backend> make_vk_backend( const vk_backend_create_info & create_info );
}  // namespace wabby::render
#endif