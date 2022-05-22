#ifndef _WABBY_VULKAN_REGISTRY_HPP
#define _WABBY_VULKAN_REGISTRY_HPP

// std
#include "any"
#include "memory"
#include "shared_mutex"
#include "unordered_map"

namespace wabby::container
{
  namespace detail
  {
    class registry_impl
    {
    public:
      registry_impl( const registry_impl & ) = delete;

      registry_impl & operator=( const registry_impl & ) = delete;

      ~registry_impl() = default;

    public:
      void sign_in( const std::string & name, std::any data );

      void sign_out( const std::string & name );

      std::any & get( const std::string & name );

    protected:
      registry_impl() = default;

    private:
      std::shared_mutex                         registry_mutex_;
      std::unordered_map<std::string, std::any> registry_data_;
    };
  }  // namespace detail

  class registry : private detail::registry_impl
  {
  public:
    registry() = default;

  public:
    template <typename T>
    void sign_in( const std::string & name, T && data )
    {
      registry_impl::sign_in( name, std::forward<T>( data ) );
    }

    template <typename T>
    void sign_out( const std::string & name )
    {
      registry_impl::sign_out( name );
    }

    template <typename T>
    T & get( const std::string & name )
    {
      return std::any_cast<T &>( registry_impl::get( name ) );
    }
  };

}  // namespace wabby::container

#endif