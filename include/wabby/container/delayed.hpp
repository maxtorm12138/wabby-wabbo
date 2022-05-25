#ifndef WABBY_CONTAINER_DELAYED_HPP
#define WABBY_CONTAINER_DELAYED_HPP

// std
#include "array"
#include "cassert"
#include "cstddef"
#include "memory"
#include "unordered_map"

namespace wabby::container
{
  template <typename T>
  class delayed
  {
  public:
    delayed(){};

    ~delayed()
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

}  // namespace wabby::container

#endif