cmake_minimum_required(VERSION 3.21.0)
project(wabbymemory VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB WABBY_MEMORY_SRCS *.cpp *.hpp)
add_library(wabbymemory STATIC ${WABBY_MEMORY_SRCS})
target_include_directories(wabbymemory PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(wabbymemory PRIVATE Boost::boost)