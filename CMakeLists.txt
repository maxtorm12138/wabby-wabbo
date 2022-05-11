cmake_minimum_required(VERSION 3.21.0)
project(wabby-waboo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

set(SDL2_BUILDING_LIBRARY ON)
find_package(SDL2 REQUIRED)
if (WIN32)
    get_filename_component(SDL2_LIBRARY_PATH ${SDL2_LIBRARIES} DIRECTORY)
    find_file(SDL2_DYNAMIC_LIBRARIES NAMES "SDL2.dll" PATHS ${SDL2_LIBRARY_PATH} REQUIRED)
    file(COPY ${SDL2_DYNAMIC_LIBRARIES} DESTINATION ${CMAKE_BINARY_DIR})
endif()

find_package(Boost REQUIRED COMPONENTS log)

find_package(Vulkan 1.3.211 EXACT REQUIRED)

add_subdirectory("contrib/VulkanMemoryAllocator")
add_subdirectory("contrib/fmt")
set(VULKAN_HPP_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/contrib/Vulkan-Hpp")

file(GLOB WAWY_UTIL_SRCS src/util/*)
add_library(wawy-util STATIC ${WAWY_UTIL_SRCS})
target_link_libraries(wawy-util PUBLIC fmt::fmt-header-only)

file(GLOB WAWY_SDL2_SRCS src/sdl2/*)
add_library(wawy-sdl SHARED ${WAWY_SDL2_SRCS})
target_compile_definitions(wawy-sdl PRIVATE -DWAWY_API_BUILDING_DLL)
target_link_libraries(wawy-sdl PUBLIC SDL2::Core wawy-util)
target_link_libraries(wawy-sdl PRIVATE fmt::fmt-header-only)
target_include_directories(wawy-sdl PRIVATE src/)
set_target_properties(wawy-sdl PROPERTIES VERSION "1.0.0" SOVERSION "1")

file(GLOB WAWY_VULKAN_SRCS src/vulkan/*)
add_library(wawy-vk SHARED ${WAWY_VULKAN_SRCS})
target_compile_definitions(wawy-vk PRIVATE -DWAWY_API_BUILDING_DLL)
target_compile_definitions(wawy-vk PRIVATE -DVULKAN_HPP_NO_SETTERS -DVULKAN_HPP_NO_SPACESHIP_OPERATOR -DVULKAN_HPP_NO_CONSTRUCTORS)
target_link_libraries(wawy-vk PRIVATE Vulkan::Vulkan Boost::boost wawy-util wawy-sdl)
target_include_directories(wawy-vk PRIVATE src/)
set_target_properties(wawy-vk PROPERTIES VERSION "1.0.0" SOVERSION "1")

file(GLOB WAWY_ENGINE_SRCS src/core/*)
add_library(wawy-engine SHARED ${WAWY_ENGINE_SRCS})
target_compile_definitions(wawy-engine PRIVATE -DWAWY_API_BUILDING_DLL)
target_link_libraries(wawy-engine PRIVATE wawy-vk wawy-sdl wawy-util)
target_include_directories(wawy-engine PRIVATE src/)
set_target_properties(wawy-engine PROPERTIES VERSION "1.0.0" SOVERSION "1")

option (WAWY_TESTS_BUILD "Build tests" OFF)
if (WAWY_TESTS_BUILD)
    add_subdirectory("contrib/googletest")
    add_subdirectory(tests)
endif ()