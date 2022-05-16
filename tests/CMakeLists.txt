cmake_minimum_required(VERSION 3.21.0)
project(wabby-waboo-test VERSION 0.1.0)

enable_testing()
include(GoogleTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_util test_util.cpp)
target_include_directories(test_util PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(test_util PUBLIC gtest_main)
gtest_discover_tests(test_util)


add_executable(test_engine test_engine.cpp)
target_include_directories(test_engine PUBLIC "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(test_engine PUBLIC wabbyengine)