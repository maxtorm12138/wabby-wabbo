#include "vk_globals.hpp"

namespace wabby::render::vulkan
{
  container::delayed<spdlog::logger> global::logger;
  fn_allocation                      global::allocation;
  fn_reallocation                    global::realllocation;
  fn_free                            global::free;

  fn_allocation::fn_allocation( pfn_allocation allocation, void * user_args ) : allocation_( allocation ), user_args_( user_args ) {}
  fn_free::fn_free( pfn_free free, void * user_args ) : free_( free ), user_args_( user_args ) {}
  fn_reallocation::fn_reallocation( pfn_reallocation reallocation, void * user_args ) : reallocation_( reallocation ), user_args_( user_args ){};

  void * fn_allocation::operator()( size_t size, size_t alignment )
  {
    return allocation_( user_args_, size, alignment );
  }

  void * fn_reallocation::operator()( void * original, size_t size, size_t alignment )
  {
    return reallocation_( user_args_, original, size, alignment );
  }

  void fn_free::operator()( void * memory )
  {
    return free_( user_args_, memory );
  }

}  // namespace wabby::render::vulkan