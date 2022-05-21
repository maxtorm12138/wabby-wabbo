#ifndef _WABBY_VULKAN_REGISTRY_HPP
#define _WABBY_VULKAN_REGISTRY_HPP

// std
#include "memory"
#include "shared_mutex"
#include "unordered_map"

namespace wabby::container
{

  class registry_impl
  {
  public:
    registry_impl( const registry_impl & ) = delete;

    registry_impl & operator=( const registry_impl & ) = delete;

    ~registry_impl() = default;

  public:
    void sign_in( const std::string & name, std::shared_ptr<void> data );

    std::shared_ptr<void> sign_out( const std::string & name );

    std::shared_ptr<void> get( const std::string & name );

  protected:
    registry_impl() = default;

  private:
    std::shared_mutex                                      registry_mutex_;
    std::unordered_map<std::string, std::shared_ptr<void>> registry_data_;
  };

  class registry : private registry_impl
  {
  public:
    registry() = default;

  public:
    template <typename T>
    void sign_in( const std::string & name, std::shared_ptr<T> data )
    {
      registry_impl::sign_in( name, std::reinterpret_pointer_cast<void>( data ) );
    }

    template <typename T>
    std::shared_ptr<T> sign_out( const std::string & name )
    {
      return std::reinterpret_pointer_cast<T>( registry_impl::sign_out( name ) );
    }

    template <typename T>
    std::shared_ptr<T> get( const std::string & name )
    {
      return std::reinterpret_pointer_cast<T>( registry_impl::get( name ) );
    }
  };

}  // namespace wabby::container

#endif