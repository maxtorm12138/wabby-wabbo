#ifndef WABBY_CONTAINER_DELAYED_HPP
#define WABBY_CONTAINER_DELAYED_HPP

// std
#include "array"
#include "cassert"
#include "concepts"
#include "cstddef"
#include "memory"
#include "vector"

namespace wabby::container
{

  namespace detail
  {
    template <typename T>
    class delayed_impl
    {
    public:
      delayed_impl(){};

      ~delayed_impl()
      {
        if ( constructed_ )
        {
          std::destroy_at( reinterpret_cast<T *>( memory_.data() ) );
        }
      }

    public:
      template <typename... Args>
      void construct( Args &&... args )
      {
        if ( constructed_ )
        {
          std::destroy_at( reinterpret_cast<T *>( memory_.data() ) );
        }
        std::construct_at( reinterpret_cast<T *>( memory_.data() ), std::forward<Args>( args )... );
        constructed_ = true;
      }

      T * operator->()
      {
        assert( constructed_ && "object shold be constructed" );
        return reinterpret_cast<T *>( memory_.data() );
      }

      T & operator*()
      {
        assert( constructed_ && "object shold be constructed" );
        return *( reinterpret_cast<T *>( memory_.data() ) );
      }

      operator T &()
      {
        assert( constructed_ && "object shold be constructed" );
        return *( reinterpret_cast<T *>( memory_.data() ) );
      }

    private:
      std::array<uint8_t, sizeof( T )> memory_{};
      bool                             constructed_{ false };
    };

  }  // namespace detail

  template <typename T, typename = void>
  struct has_subscript : std::false_type
  {
  };

  template <typename T>
  struct has_subscript<T, std::void_t<decltype( ( *std::declval<T *>() )[std::declval<size_t>()] )>> : std::true_type
  {
  };

  template <typename T, typename E = void>
  class delayed : public detail::delayed_impl<T>
  {
  public:
  };

  template <typename T>
  class delayed<T, std::enable_if_t<has_subscript<T>::value>> : public detail::delayed_impl<T>
  {
  public:
    decltype( ( *std::declval<T *>() )[std::declval<size_t>()] ) operator[]( size_t pos )
    {
      return static_cast<T &>( *this )[pos];
    }
  };

}  // namespace wabby::container

#endif