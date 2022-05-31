#include "wabby/memory/memory.hpp"

// std
#include "memory.h"

// boost
#include "boost/align/aligned_alloc.hpp"

namespace wabby::memory
{
  void * allocation( size_t size, size_t alignment )
  {
    return boost::alignment::aligned_alloc( alignment == 0 ? sizeof( void * ) : alignment, size );
  }

  void * reallocation( void * original_memory, size_t size, size_t alignment )
  {
    auto new_memory = boost::alignment::aligned_alloc( alignment == 0 ? sizeof( void * ) : alignment, size );
    if ( original_memory != nullptr )
    {
      memcpy( new_memory, original_memory, size );
      boost::alignment::aligned_free( original_memory );
    }
    return new_memory;
  }

  void free( void * memory )
  {
    boost::alignment::aligned_free( memory );
  }
}  // namespace wabby::memory