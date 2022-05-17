#include "vk_shader.hpp"

namespace wabby::render::vulkan
{

vk_shader::vk_shader(const std::string &name, vk::ShaderStageFlagBits stage) :
    name_(name),
    stage_(stage),
    module_(nullptr)
{}

}