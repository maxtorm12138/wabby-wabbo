#ifndef _WABBY_CONTAINER_DYNAMIC_ARRAY_HPP
#define _WABBY_CONTAINER_DYNAMIC_ARRAY_HPP

namespace wabby::container
{

  template <typename T>
  class dynamic_array
  {
  public:
    dynamic_array();

    ~dynamic_array();

  public:
    void push_back( T && t );

    void pop_back();

    size_t size() const;

  private:
    void realloc( size_t size );

  private:
    size_t size_;
    size_t capacity_;
    T *    data_;
  };

}  // namespace wabby::container

#endif