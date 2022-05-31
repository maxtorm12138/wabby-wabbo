#ifndef WABBY_MEMORY_MEMORY_HPP
#define WABBY_MEMORY_MEMORY_HPP

#include "cstddef"

namespace wabby::memory
{
  void * allocation( size_t size, size_t alignment );

  void * reallocation( void * original_memory, size_t size, size_t alignment );

  void free( void * memory );
}  // namespace wabby::memory
#endif
