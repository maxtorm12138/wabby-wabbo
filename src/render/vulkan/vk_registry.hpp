#ifndef _WABBY_VULKAN_REGISTRY_HPP
#define _WABBY_VULKAN_REGISTRY_HPP

// std
#include "mutex"

// comm includes
#include "vk_comm_include.hpp"

namespace wabby::render::vulkan
{

  class registry_impl : public boost::noncopyable
  {
  public:
    void sign_in( const std::string & name, std::shared_ptr<void *> data );

    std::shared_ptr<void *> sign_out( const std::string & name );

  protected:
    registry_impl() = default;

  private:
    std::mutex                                               registry_mutex_;
    std::unordered_map<std::string, std::shared_ptr<void *>> registry_data_;
  };

  template <typename T>
  class registry : private registry_impl
  {
  public:
    void sign_in( const std::string & name, std::shared_ptr<T> data )
    {
      registry_impl::sign_in( name, std::static_pointer_cast<void>( data ) );
    }

    std::shared_ptr<T> sign_out( const std::string & name )
    {
      return std::static_pointer_cast<T>( registry_impl::sign_out( name ) );
    }

    static registry<T> instance()
    {
      static registry<T> instance;
      return instance;
    }

  private:
    registry() = default;
  };

}  // namespace wabby::render::vulkan

#endif