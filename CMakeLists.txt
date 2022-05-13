cmake_minimum_required(VERSION 3.21.0)
project(wabby-waboo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

set(SDL2_BUILDING_LIBRARY ON)
find_package(SDL2 REQUIRED)
if (WIN32)
    get_filename_component(SDL2_LIBRARY_PATH ${SDL2_LIBRARIES} DIRECTORY)
    find_file(SDL2_DYNAMIC_LIBRARIES NAMES "SDL2.dll" PATHS ${SDL2_LIBRARY_PATH} REQUIRED)
    file(COPY ${SDL2_DYNAMIC_LIBRARIES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

find_package(Boost REQUIRED COMPONENTS log date_time)

find_package(Vulkan 1.3.211 EXACT REQUIRED)

add_subdirectory("contrib/fmt")
set(VMA_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/contrib/VulkanMemoryAllocator/include")
set(VULKAN_HPP_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/contrib/Vulkan-Hpp")

file(GLOB WABBY_UTIL_SRCS src/util/*)
add_library(wabbyutil STATIC ${WABBY_UTIL_SRCS})
target_link_libraries(wabbyutil PUBLIC fmt::fmt-header-only)
target_link_libraries(wabbyutil PRIVATE Boost::date_time)

file(GLOB WABBY_SDL2_SRCS src/sdl2/*)
add_library(wabbysdl SHARED ${WABBY_SDL2_SRCS})
target_compile_definitions(wabbysdl PRIVATE -DWABBY_API_BUILDING_DLL)
target_link_libraries(wabbysdl PRIVATE SDL2::Core wabbyutil fmt::fmt-header-only)
target_include_directories(wabbysdl PRIVATE src/)
target_include_directories(wabbysdl PUBLIC ${SDL2_INCLUDE_DIRS})
set_target_properties(wabbysdl PROPERTIES VERSION "1.0.0" SOVERSION "1")

file(GLOB WABBY_VULKAN_SRCS src/vulkan/*)
add_library(wabbyvk SHARED ${WABBY_VULKAN_SRCS})
target_compile_definitions(wabbyvk PRIVATE -DWABBY_API_BUILDING_DLL)
target_compile_definitions(wabbyvk PRIVATE -DVULKAN_HPP_NO_SETTERS -DVULKAN_HPP_NO_SPACESHIP_OPERATOR -DVULKAN_HPP_NO_CONSTRUCTORS)
target_link_libraries(wabbyvk PRIVATE Vulkan::Vulkan Boost::boost wabbyutil fmt::fmt-header-only)
target_include_directories(wabbyvk PRIVATE src/ ${VMA_INCLUDE_DIR} ${VULKAN_HPP_INCLUDE_DIR})
set_target_properties(wabbyvk PROPERTIES VERSION "1.0.0" SOVERSION "1")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(wabbyvk PRIVATE -Wno-nullability-completeness)
endif ()

file(GLOB WABBY_ENGINE_SRCS src/core/*)
add_library(wabbyengine SHARED ${WABBY_ENGINE_SRCS})
target_compile_definitions(wabbyengine PRIVATE -DWABBY_API_BUILDING_DLL)
target_link_libraries(wabbyengine PRIVATE wabbyvk wabbysdl wabbyutil)
target_include_directories(wabbyengine PRIVATE src/)
set_target_properties(wabbyengine PROPERTIES VERSION "1.0.0" SOVERSION "1")

option (WABBY_TESTS_BUILD "Build tests" OFF)
if (WABBY_TESTS_BUILD)
    add_subdirectory("contrib/googletest")
    add_subdirectory(tests)
endif ()