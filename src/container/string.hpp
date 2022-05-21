#ifndef _WABBY_CONTAINER_STRING_HPP
#define _WABBY_CONTAINER_STRING_HPP

#include "compare"

namespace wabby::container
{
  namespace detail
  {
    constexpr auto small_string_length = 15;

    struct string_large
    {
      size_t capacity;
      char * data;
    };

    using string_sso = char[small_string_length + 1];
  }  // namespace detail

  class string
  {
  public:
    string( const char * str );

    string( const char * str, size_t n );

    string( const string & other );

    ~string();

  public:
    const char * c_str() const noexcept;

    size_t size() const noexcept;

  private:
    size_t size_;
    union
    {
      detail::string_large large;
      detail::string_sso   small;
    } data_;
  };

  std::strong_ordering operator<=>( const string & lsh, const string & rsh );
}  // namespace wabby::container

#endif