cmake_minimum_required(VERSION 3.20)

add_library(spark STATIC
    src/spark.c
    src/render/shader.c
    src/platform/x11/x11.c
    src/glad/glad.c
)

target_include_directories(spark PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

target_link_libraries(spark PUBLIC
    base
    OpenGL::GL
    X11::X11
)

target_compile_options(spark PRIVATE
    -Wall
    -Wextra
)
