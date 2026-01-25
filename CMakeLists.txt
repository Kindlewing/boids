cmake_minimum_required(VERSION 3.20)
project(boids)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(boids src/main.c)
target_include_directories(boids PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(lib/base)
add_subdirectory(lib/spark)


find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)


target_link_libraries(boids PRIVATE 
	base
	spark 
	OpenGL::GL
    X11::X11
)
target_compile_options(boids PRIVATE -Wall -Wextra)
