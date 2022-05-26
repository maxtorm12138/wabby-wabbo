#include "vk_shader.hpp"

#include "vk_allocator.hpp"

namespace wabby::render::vulkan
{

  vk_shader
    vk_shader::from_file( const vk::raii::Device & device, const std::filesystem::path & path, const std::string & entry, vk::ShaderStageFlagBits stage )
  {
    if ( !std::filesystem::exists( path ) )
    {
      throw vk::LogicError( fmt::format( "vk_shader::from_file path: {} not exits", path.string() ) );
    }

    std::ifstream shader_code_file( path.c_str(), std::ios::ate | std::ios::binary );
    if ( !shader_code_file.is_open() )
    {
      throw vk::LogicError( fmt::format( "vk_shader::from_file {} open fail", path.string() ) );
    }

    std::vector<char, vk_allocator<char, 8>> shader_code( shader_code_file.tellg() );
    shader_code_file.seekg( 0 );
    shader_code_file.read( shader_code.data(), shader_code.size() );

    vk::ShaderModuleCreateInfo shader_module_create_info{ .codeSize = shader_code.size(), .pCode = reinterpret_cast<uint32_t *>( shader_code.data() ) };

    vk_shader tmp_shader;
    tmp_shader.entry_  = entry;
    tmp_shader.stage_  = stage;
    tmp_shader.module_ = vk::raii::ShaderModule( device, shader_module_create_info );
    return std::move( tmp_shader );
  }

  vk_shader::vk_shader( vk_shader && other ) noexcept
    : entry_( std::move( other.entry_ ) ), stage_( std::move( other.stage_ ) ), module_( std::move( other.module_ ) )
  {
  }

  vk_shader & vk_shader::operator=( vk_shader && other ) noexcept
  {
    entry_  = std::move( other.entry_ );
    stage_  = std::move( other.stage_ );
    module_ = std::move( other.module_ );
    return *this;
  }
}  // namespace wabby::render::vulkan