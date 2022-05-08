cmake_minimum_required(VERSION 3.21.0)
project(VulkanMemoryAllocator VERSION 3.0.0)

# vulkan sdk
find_package(Vulkan REQUIRED)

# vma static
add_library(vma STATIC vk_mem_alloc.h vk_mem_alloc.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(vma PRIVATE -Wno-nullability-completeness)
endif ()

target_link_libraries(vma PUBLIC Vulkan::Headers)
add_library(vma::vma ALIAS vma)