cmake_minimum_required(VERSION 3.21.0)
project(wabby-waboo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

add_subdirectory("contrib/VulkanMemoryAllocator")