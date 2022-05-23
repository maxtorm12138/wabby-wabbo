cmake_minimum_required(VERSION 3.21.0)
project(wabbyvulkan VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB WABBY_VULKAN_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/vulkan/vk_*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/vulkan/vk_*.hpp)
add_library(wabbyvulkan MODULE ${WABBY_VULKAN_SRCS})

# vulkan hpp defs
target_compile_definitions(wabbyvulkan PRIVATE -DVULKAN_HPP_NO_SETTERS -DVULKAN_HPP_NO_SPACESHIP_OPERATOR -DVULKAN_HPP_NO_CONSTRUCTORS)
# boost defs
target_compile_definitions(wabbyvulkan PRIVATE -DBOOST_DLL_USE_STD_FS)
# glm defs
target_compile_definitions(wabbyvulkan PRIVATE -DGLM_FORCE_RADIANS -DGLM_FORCE_DEPTH_ZERO_TO_ONE)

target_link_libraries(wabbyvulkan PRIVATE Vulkan::Vulkan Boost::boost fmt::fmt-header-only vma vulkanhpp spdlog::spdlog_header_only glm::glm PUBLIC wabbycontainer)

target_include_directories(wabbyvulkan PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/vulkan)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(wabbyvulkan PRIVATE -fvisibility=hidden -fno-rtti -W -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(wabbyvulkan PRIVATE /GR-)
endif ()