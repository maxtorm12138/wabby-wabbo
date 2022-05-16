cmake_minimum_required(VERSION 3.21.0)
project(wabbysdl VERSION 1.0.0)


file(GLOB WABBY_SDL_SRCS *.hpp *.cpp)
add_library(wabbysdl SHARED ${WABBY_SDL_SRCS})
target_link_libraries(wabbysdl PRIVATE SDL2::Core fmt::fmt-header-only Boost::boost)
target_include_directories(wabbysdl PRIVATE src/)
target_include_directories(wabbysdl PUBLIC ${SDL2_INCLUDE_DIRS})
set_target_properties(wabbysdl PROPERTIES VERSION "1.0.0" SOVERSION "1")