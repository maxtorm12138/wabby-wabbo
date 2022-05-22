#include "registry.hpp"
namespace wabby::container::detail
{

  void registry_impl::sign_in( const std::string & name, std::any data )
  {
    std::unique_lock<std::shared_mutex> guard( registry_mutex_ );
    if ( registry_data_.contains( name ) )
    {
      throw std::runtime_error( "" );
    }

    registry_data_.emplace( name.data(), data );
  }

  void registry_impl::sign_out( const std::string & name )
  {
    std::unique_lock<std::shared_mutex> guard( registry_mutex_ );
    if ( !registry_data_.contains( name ) )
    {
      throw std::runtime_error( "" );
    }

    registry_data_.extract( name ).mapped();
  }

  std::any & registry_impl::get( const std::string & name )
  {
    static std::any CACHED_NULL_ANY( nullptr );

    std::shared_lock<std::shared_mutex> guard( registry_mutex_ );

    if ( auto index = registry_data_.find( name ); index != registry_data_.end() )
    {
      return index->second;
    }

    return CACHED_NULL_ANY;
  }

}  // namespace wabby::container::detail