#ifndef WABBY_CONTAINER_DELAYED_HPP
#define WABBY_CONTAINER_DELAYED_HPP

// std
#include "array"
#include "cassert"
#include "cstddef"
#include "memory"

namespace wabby::container
{

  template <typename T>
  class delayed
  {
  public:
    delayed(){};

    ~delayed()
    {
      assert( constructed_ && "object shold be constructed" );
      std::destroy_at( reinterpret_cast<T *>( memory_.data() ) );
    }

  public:
    template <typename... Args>
    void construct( Args &&... args )
    {
      assert( !constructed_ && "construct shold be called once" );
      std::construct_at( reinterpret_cast<T *>( memory_.data() ), std::forward<Args>( args )... );
#ifndef NDEBUG
      constructed_ = true;
#endif
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
      return *( reinterpret_cast<T *>( memory_ ) );
    }

  private:
    std::array<uint8_t, sizeof( T )> memory_{};
#ifndef NDEBUG
    bool constructed_{ false };
#endif
  };
}  // namespace wabby::container

#endif