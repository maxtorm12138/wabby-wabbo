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
  namespace detail
  {
    class delayed_tracer
    {
    protected:
      void test_constructed();

      void test_not_constructed();

      void set_constructed();

    protected:
#ifndef NDEBUG
      static std::unordered_map<void *, bool> constructed_;
#endif
    };
  }  // namespace detail

  template <typename T>
  class delayed : public detail::delayed_tracer
  {
  public:
    delayed(){};

    ~delayed()
    {
      test_constructed();
      std::destroy_at( reinterpret_cast<T *>( memory_.data() ) );
    }

  public:
    template <typename... Args>
    void construct( Args &&... args )
    {
      test_not_constructed();
      std::construct_at( reinterpret_cast<T *>( memory_.data() ), std::forward<Args>( args )... );
      set_constructed();
    }

    T * operator->()
    {
      test_constructed();
      return reinterpret_cast<T *>( memory_.data() );
    }

    T & operator*()
    {
      test_constructed();
      return *( reinterpret_cast<T *>( memory_.data() ) );
    }

    operator T &()
    {
      test_constructed();
      return *( reinterpret_cast<T *>( memory_.data() ) );
    }

  private:
    std::array<uint8_t, sizeof( T )> memory_{};
  };

}  // namespace wabby::container

#endif