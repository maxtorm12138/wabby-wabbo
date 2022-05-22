#include "string.hpp"

#include "cstring"

namespace wabby::container
{
  string::string() : size_( 0 ){};

  string::string( const char * str ) : string( str, strlen( str ) ) {}

  string::string( const char * str, size_t n ) : size_( n )
  {
    if ( size_ <= detail::small_string_length )
    {
      memcpy( data_.small, str, size_ + 1 );
    }
    else
    {
      data_.large.data     = new char[size_ + 1];
      data_.large.capacity = size_ + 1;
      memcpy( data_.large.data, str, size_ + 1 );
    }
  }

  string::string( const string & other ) : size_( other.size_ )
  {
    if ( size_ <= detail::small_string_length )
    {
      memcpy( data_.small, other.data_.small, size_ + 1 );
    }
    else
    {
      data_.large.data     = new char[size_ + 1];
      data_.large.capacity = size_ + 1;
      memcpy( data_.large.data, other.data_.large.data, size_ + 1 );
    }
  }

  string & string::operator=( const string & other )
  {
    if ( size_ > detail::small_string_length )
    {
      delete[] data_.large.data;
    }

    size_ = other.size_;
    if ( size_ <= detail::small_string_length )
    {
      memcpy( data_.small, other.data_.small, size_ + 1 );
    }
    else
    {
      data_.large.data     = new char[size_ + 1];
      data_.large.capacity = size_ + 1;
      memcpy( data_.large.data, other.data_.large.data, size_ + 1 );
    }

    return *this;
  }

  string::~string()
  {
    if ( size_ <= detail::small_string_length )
    {
      // DO NOTHING
    }
    else
    {
      delete[] data_.large.data;
    }
  }

  const char * string::c_str() const noexcept
  {
    if ( size_ <= detail::small_string_length )
    {
      return data_.small;
    }
    else
    {
      return data_.large.data;
    }
  }

  size_t string::size() const noexcept
  {
    return size_;
  }

  std::strong_ordering operator<=>( const string & lsh, const string & rsh )
  {
    auto cmp = strcmp( lsh.c_str(), rsh.c_str() );
    if ( cmp == 0 )
    {
      return std::strong_ordering::equal;
    }
    else if ( cmp < 0 )
    {
      return std::strong_ordering::less;
    }
    else
    {
      return std::strong_ordering::greater;
    }
  }
}  // namespace wabby::container