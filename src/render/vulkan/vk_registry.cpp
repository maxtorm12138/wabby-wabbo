#include "vk_registry.hpp"
namespace wabby::render::vulkan
{

void registry_impl::sign_in(const std::string &name, std::shared_ptr<void *> data)
{
    std::lock_guard<std::mutex> guard(registry_mutex_);
    if (registry_data_.contains(name))
    {
        throw std::runtime_error("");
    }

    registry_data_.emplace(name.data(), data);
}

std::shared_ptr<void *> registry_impl::sign_out(const std::string &name)
{
    std::lock_guard<std::mutex> guard(registry_mutex_);
    if (!registry_data_.contains(name))
    {
        throw std::runtime_error("");
    }

    return registry_data_.extract(name).mapped();
}

}