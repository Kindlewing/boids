cmake_minimum_required(VERSION 3.20)

add_library(base STATIC
    src/mem/arena.c
    src/str/string8.c
)

target_include_directories(base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(base PRIVATE
    -Wall
    -Wextra
)
