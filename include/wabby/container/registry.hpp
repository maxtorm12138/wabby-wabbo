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

      std::any * const get( const std::string & name );

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
      registry_impl::sign_in( name, std::make_any<T>( std::forward<T>( data ) ) );
    }

    template <typename T>
    void sign_out( const std::string & name )
    {
      registry_impl::sign_out( name );
    }

    template <typename T>
    T * const get( const std::string & name )
    {
      auto item = registry_impl::get( name );
      if ( item == nullptr )
      {
        return nullptr;
      }
      auto ret = std::any_cast<T>( item );
      return ret;
    }
  };

}  // namespace wabby::container

#endif