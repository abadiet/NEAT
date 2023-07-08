cmake_minimum_required(VERSION 3.0)

project(neat)

file(GLOB SOURCES src/*.cpp)

include_directories(./include)

add_library(neat STATIC ${SOURCES})

target_compile_options(neat PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Werror -O3 -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wunreachable-code -Wconversion -Wno-error=maybe-uninitialized -Wpedantic -Wcast-align -Wno-error=vla>
)

target_include_directories(neat PUBLIC include/)