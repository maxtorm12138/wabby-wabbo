#include "wabby/sdl2/window.hpp"

// module
#include "wabby/sdl2/error.hpp"

// std
#include "iterator"

namespace wabby::sdl2
{

  window::window( const char * title, int w, int h, int x, int y, uint32_t flags ) : window_()
  {
    window_ = SDL_CreateWindow( title, x, y, w, h, flags );
    SDL2_ASSERT( window_ != nullptr, SDL_CreateWindow );
  }

  window::window( window && other ) noexcept : window_( std::exchange( other.window_, window_ ) ) {}

  window & window::operator=( window && other ) noexcept
  {
    std::swap( window_, other.window_ );
    return *this;
  }

  window::~window()
  {
    if ( window_ != nullptr )
    {
      SDL_DestroyWindow( window_ );
    }
  }

  void window::show() noexcept
  {
    SDL_ShowWindow( window_ );
  }

  std::vector<const char *> window::get_vulkan_instance_extensions() const
  {
    unsigned count{ 0 };
    auto     ret = SDL_Vulkan_GetInstanceExtensions( window_, &count, nullptr );
    SDL2_ASSERT( ret == SDL_TRUE, SDL_Vulkan_GetInstanceExtensions );
    std::vector<const char *> instance_extensions( count );

    ret = SDL_Vulkan_GetInstanceExtensions( window_, &count, instance_extensions.data() );
    SDL2_ASSERT( ret == SDL_TRUE, SDL_Vulkan_GetInstanceExtensions );

    return instance_extensions;
  }

  VkSurfaceKHR window::create_vulkan_surface( VkInstance instance ) const
  {
    VkSurfaceKHR surface;
    auto         ret = SDL_Vulkan_CreateSurface( window_, instance, &surface );
    SDL2_ASSERT( ret == SDL_TRUE, SDL_Vulkan_CreateSurface );
    return surface;
  }

  std::pair<uint32_t, uint32_t> window::get_vulakn_drawable_size() const
  {
    int w{ 0 };
    int h{ 0 };
    SDL_Vulkan_GetDrawableSize( window_, &w, &h );
    return std::make_pair( static_cast<uint32_t>( w ), static_cast<uint32_t>( h ) );
  }

  void window::set_resizeable( bool resizable ) const
  {
    SDL_SetWindowResizable( window_, resizable ? SDL_TRUE : SDL_FALSE );
  }

}  // namespace wabby::sdl2