cmake_minimum_required(VERSION 3.20)
project(boids LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib/base)
add_subdirectory(lib/spark)

add_executable(boids src/main.c)
target_include_directories(boids PRIVATE include)

target_link_libraries(boids PRIVATE
    base
    spark
)

target_compile_options(boids PRIVATE
    $<$<C_COMPILER_ID:GNU,Clang>:-Wall -Wextra>
    $<$<C_COMPILER_ID:MSVC>:/W4>
)
