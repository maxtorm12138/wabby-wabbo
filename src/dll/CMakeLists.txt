cmake_minimum_required(VERSION 3.21.0)
project(wabbydll VERSION 1.0.0)

file(GLOB WABBY_DLL_SRCS *.hpp *.cpp)
add_library(wabbydll STATIC ${WABBY_DLL_SRCS})
target_link_libraries(wabbydll PRIVATE Boost::boost)