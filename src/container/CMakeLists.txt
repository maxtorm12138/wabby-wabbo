cmake_minimum_required(VERSION 3.21.0)
project(wabbycontainer VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB WABBY_CONTAINER_SRCS *.cpp *.hpp ${CMAKE_SOURCE_DIR}/include/container/*.hpp)
add_library(wabbycontainer STATIC ${WABBY_CONTAINER_SRCS})
target_include_directories(wabbycontainer PRIVATE ${CMAKE_SOURCE_DIR}/include)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(wabbycontainer PRIVATE -fno-rtti -Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(wabbycontainer PRIVATE /GR- /Wall)
endif ()