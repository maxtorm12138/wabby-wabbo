#ifndef WABBY_CONTAINER_DELAYED_HPP
#define WABBY_CONTAINER_DELAYED_HPP

// std
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
      std::destroy_at( reinterpret_cast<T *>( memory_ ) );
    }

  public:
    template <typename... Args>
    void construct( Args &&... args )
    {
      assert( !constructed_ && "construct shold be called once" );
      std::construct_at( reinterpret_cast<T *>( memory_ ), std::forward<Args>( args )... );
#ifndef NDEBUG
      constructed_ = true;
#endif
    }

    T * operator->()
    {
      assert( constructed_ && "object shold be constructed" );
      return reinterpret_cast<T *>( memory_ );
    }

    T & operator*()
    {
      assert( constructed_ && "object shold be constructed" );
      return *( reinterpret_cast<T *>( memory_ ) );
    }

    operator T &()
    {
      assert( constructed_ && "object shold be constructed" );
      return *( reinterpret_cast<T *>( memory_ ) );
    }

  private:
    std::byte memory_[sizeof( T )]{};
#ifndef NDEBUG
    bool constructed_{ false };
#endif
  };
}  // namespace wabby::container

#endif