#ifndef _WABBY_RENDERER_RENDERER_HPP
#define _WABBY_RENDERER_RENDERER_HPP

// container
#include "container/registry.hpp"
#include "container/string.hpp"

// boost
#include "boost/config.hpp"
#include "boost/noncopyable.hpp"

// std
#include "functional"
#include "vector"

extern "C"
{
  typedef struct VkSurfaceKHR_T * VkSurfaceKHR;
  typedef struct VkInstance_T *   VkInstance;
}

namespace wabby::render
{
  struct BOOST_SYMBOL_EXPORT backend_create_info
  {
    virtual ~backend_create_info() = default;

    const char *                         applicaiton_name;
    uint32_t                             application_version;
    std::shared_ptr<container::registry> registry;
  };

  struct BOOST_SYMBOL_EXPORT vk_backend_create_info : public backend_create_info
  {
    std::vector<std::string>                       windowsystem_extensions;
    std::function<VkSurfaceKHR( VkInstance )>      fn_make_surface;
    std::function<std::pair<uint32_t, uint32_t>()> fn_get_window_size;
  };

  class BOOST_SYMBOL_EXPORT backend : boost::noncopyable
  {
  public:
    backend()          = default;
    virtual ~backend() = default;

  public:
    virtual void setup( const backend_create_info & create_info ) = 0;

    virtual void begin_frame() = 0;

    virtual void end_frame() = 0;

    virtual void begin_render_pass() = 0;

    virtual void end_render_pass() = 0;

    virtual void resized() = 0;

    virtual void teardown() = 0;
  };

  BOOST_SYMBOL_EXPORT std::unique_ptr<backend> make_vk_backend();
}  // namespace wabby::render
#endif
