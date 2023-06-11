cmake_minimum_required(VERSION 3.0)

project(SnakeNEAT)

add_executable(SnakeNEAT main.cpp) 

target_compile_options(SnakeNEAT PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Werror -O3 -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wunreachable-code -Wconversion -Wno-error=maybe-uninitialized -Wpedantic -Wcast-align>
)

include_directories(../../include)

add_library(neat STATIC IMPORTED)
set_property(TARGET neat PROPERTY IMPORTED_LOCATION ../../../lib/libneat.a)

target_link_libraries(SnakeNEAT neat sfml-window sfml-system sfml-graphics)
