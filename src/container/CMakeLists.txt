cmake_minimum_required(VERSION 3.21.0)
project(wabbycontainer VERSION 1.0.0)

file(GLOB WABBY_CONTAINER_SRCS *.cpp *.hpp)
add_library(wabbycontainer STATIC ${WABBY_CONTAINER_SRCS})