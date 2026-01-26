cmake_minimum_required(VERSION 3.20)

add_library(base STATIC
    src/arena.c
    src/string8.c
	src/math.c
)

target_include_directories(base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(base PRIVATE
    -Wall
    -Wextra
)
