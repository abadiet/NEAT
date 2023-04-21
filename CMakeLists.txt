cmake_minimum_required(VERSION 3.0)

project(neat)

file(GLOB SOURCES src/*.cpp)

include_directories(./include)

add_definitions("-Wall -Wextra -Werror -O3 -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wunreachable-code -Wconversion -Wno-error=maybe-uninitialized")

add_library(neat STATIC ${SOURCES})

target_include_directories(neat PUBLIC include/)