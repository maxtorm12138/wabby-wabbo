cmake_minimum_required(VERSION 3.21.0)
project(wabby-waboo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

find_package(Boost REQUIRED COMPONENTS log date_time filesystem system)
find_package(Vulkan 1.3.211 EXACT REQUIRED)
add_library(vma INTERFACE)
target_include_directories(vma INTERFACE "${PROJECT_SOURCE_DIR}/contrib/VulkanMemoryAllocator/include")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(vma INTERFACE -Wno-nullability-completeness)
endif ()

add_library(vulkanhpp INTERFACE)
target_include_directories(vulkanhpp INTERFACE "${PROJECT_SOURCE_DIR}/contrib/Vulkan-Hpp")

set(SDL2_BUILDING_LIBRARY ON)
find_package(SDL2 REQUIRED)
if (WIN32)
    get_filename_component(SDL2_LIBRARY_PATH ${SDL2_LIBRARIES} DIRECTORY)
    find_file(SDL2_DYNAMIC_LIBRARIES NAMES "SDL2.dll" PATHS ${SDL2_LIBRARY_PATH} REQUIRED)
    file(COPY ${SDL2_DYNAMIC_LIBRARIES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

add_subdirectory("contrib/fmt")
add_subdirectory("contrib/spdlog")
add_subdirectory("contrib/glm")
add_subdirectory("contrib/inipp")
add_subdirectory("src/container")
add_subdirectory("src/sdl")
add_subdirectory("src/render")


file(GLOB WABBY_ENGINE_SRCS src/core/*.cpp src/core/*.hpp)
add_library(wabbyengine SHARED ${WABBY_ENGINE_SRCS})
target_compile_definitions(wabbyengine PRIVATE -DBOOST_DLL_USE_STD_FS)
target_link_libraries(wabbyengine PRIVATE wabbysdl spdlog::spdlog_header_only PUBLIC Boost::boost wabbycontainer)
target_include_directories(wabbyengine PRIVATE src/)
add_dependencies(wabbyengine wabbyvulkan)

option (WABBY_TESTS_BUILD "Build tests" OFF)
if (WABBY_TESTS_BUILD)
    add_subdirectory("contrib/googletest")
    add_subdirectory(tests)
endif ()