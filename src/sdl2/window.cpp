#include "window.hpp"

// module
#include "error.hpp"

namespace wawy::sdl2
{

window::window(
    std::string_view title,
    int w,
    int h,
    int x,
    int y,
    uint32_t flags) :
    window_()
{
    window_ = SDL_CreateWindow(title.data(), x, y, w, h, flags);
    SDL2_ASSERT(window_ != nullptr, SDL_CreateWindow);
}


window::window(window &&other) noexcept :
    window_(std::exchange(other.window_, window_))
{}

window &window::operator=(window &&other) noexcept
{
    std::swap(window_, other.window_);
    return *this;
}

window::~window()
{
    if (window_ != nullptr)
    {
        SDL_DestroyWindow(window_);
    }
}

void window::show()
{
    SDL_ShowWindow(window_);
}

std::vector<std::string> window::get_vulkan_instance_extensions() const
{
    std::vector<std::string> result;

    unsigned count{0};
    auto ret = SDL_Vulkan_GetInstanceExtensions(window_, &count, nullptr);
    SDL2_ASSERT(ret == SDL_TRUE, SDL_Vulkan_GetInstanceExtensions);
    std::vector<const char *> instance_extensions(count);
    result.reserve(count);

    ret = SDL_Vulkan_GetInstanceExtensions(window_, &count, instance_extensions.data());
    SDL2_ASSERT(ret == SDL_TRUE, SDL_Vulkan_GetInstanceExtensions);
    std::copy(instance_extensions.begin(), instance_extensions.end(), std::back_inserter(result));
    return result;
}

VkSurfaceKHR window::create_vulkan_surface(VkInstance instance) const
{
    VkSurfaceKHR surface;
    auto ret = SDL_Vulkan_CreateSurface(window_, instance, &surface);
    SDL2_ASSERT(ret == SDL_TRUE, SDL_Vulkan_CreateSurface);
	return surface;
}

}