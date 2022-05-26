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

  template <class T, class I = std::size_t>
  concept Indexible = requires( T & t, const I & i )
  {
    { t[i] };
  };

  template <typename T>
  class delayed : public detail::delayed_impl<T>
  {
  public:
  };

  template <Indexible T>
  class delayed : public detail::delayed_impl<T>
  {
  public:
    T & operator[]( size_t pos )
    {
      return static_cast<std::vector<T, Alloc> &>( *this )[pos];
    }
  };

}  // namespace wabby::container

#endif