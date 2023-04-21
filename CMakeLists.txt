cmake_minimum_required(VERSION 3.0)

project(neat)

file(GLOB SOURCES src/*.cpp)

include_directories(./include)

add_library(neat STATIC ${SOURCES})

target_include_directories(neat PUBLIC include/)