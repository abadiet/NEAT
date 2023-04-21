cmake_minimum_required(VERSION 3.0)

project(SnakeNEAT)

include_directories(../../include)
add_executable(SnakeNEAT main.cpp) 

add_library(neat STATIC IMPORTED)
set_property(TARGET neat PROPERTY IMPORTED_LOCATION ../../../lib/libneat.a)

target_link_libraries(SnakeNEAT neat sfml-window sfml-system sfml-graphics)