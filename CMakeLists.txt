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

add_subdirectory("contrib/VulkanMemoryAllocator")

file(GLOB WAWY_SDL2_SRCS src/sdl2/*)
add_library(wawy-sdl2 ${WAWY_SDL2_SRCS})
target_link_libraries(wawy-sdl2 PRIVATE SDL2::Core)
target_include_directories(wawy-sdl2 PRIVATE src/)