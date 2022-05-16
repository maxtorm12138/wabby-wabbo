cmake_minimum_required(VERSION 3.21.0)
project(wabbyrenderer VERSION 1.0.0)

file(GLOB WABBY_VULKAN_SRCS vulkan/vk_*.cpp vulkan/vk_*.hpp)
add_library(wabbyvulkan SHARED ${WABBY_VULKAN_SRCS})
target_compile_definitions(wabbyvulkan PRIVATE -DVULKAN_HPP_NO_SETTERS -DVULKAN_HPP_NO_SPACESHIP_OPERATOR -DVULKAN_HPP_NO_CONSTRUCTORS)
target_link_libraries(wabbyvulkan PRIVATE Vulkan::Vulkan Boost::boost fmt::fmt-header-only vma vulkanhpp glog::glog)
target_include_directories(wabbyvulkan PRIVATE ./)

set_target_properties(wabbyvulkan PROPERTIES VERSION "1.0.0" SOVERSION "1")