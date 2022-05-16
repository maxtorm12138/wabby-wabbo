cmake_minimum_required(VERSION 3.21.0)
project(wabbyrenderer VERSION 1.0.0)

file(GLOB WABBY_VULKAN_SRCS vulkan/*.cpp vulkan/*.hpp)
add_library(wabbyvulkan SHARED ${WABBY_VULKAN_SRCS})
target_compile_definitions(wabbyvulkan PRIVATE -DWABBY_API_BUILDING_DLL)
target_compile_definitions(wabbyvulkan PRIVATE -DVULKAN_HPP_NO_SETTERS -DVULKAN_HPP_NO_SPACESHIP_OPERATOR -DVULKAN_HPP_NO_CONSTRUCTORS)
target_link_libraries(wabbyvulkan PRIVATE Vulkan::Vulkan Boost::boost wabbyutil fmt::fmt-header-only vma vulkanhpp)

set_target_properties(wabbyvulkan PROPERTIES VERSION "1.0.0" SOVERSION "1")