#ifndef WABBY_CONTAINER_DELAYED_HPP
#define WABBY_CONTAINER_DELAYED_HPP

// std
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
      std::destroy_at( reinterpret_cast<T *>( memory_ ) );
    }

  public:
    template <typename... Args>
    void emplace( Args &&... args )
    {
      std::construct_at( reinterpret_cast<T *>( memory_ ), std::forward<Args>( args )... );
    }

    T * operator->()
    {
      return reinterpret_cast<T *>( memory_ );
    }

    T & operator*()
    {
      return *reinterpret_cast<T *>( memory_ );
    }

  private:
    std::byte memory_[sizeof( T )]{};
  };

}  // namespace wabby::container

#endif