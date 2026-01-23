cmake_minimum_required(VERSION 3.20)
project(boids)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Include root include if needed for any third-party headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add local libraries
add_subdirectory(lib/base)
add_subdirectory(lib/spark)

# Main executable
add_executable(boids src/main.c)

# Link local libraries
target_link_libraries(boids PRIVATE base spark GL X11)

# Compiler flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(boids PRIVATE -Wall -Wextra -g -O0)
else()
    target_compile_options(boids PRIVATE -O3)
endif()

