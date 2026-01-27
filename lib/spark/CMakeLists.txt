cmake_minimum_required(VERSION 3.20)

set(SPARK_SOURCES
    src/spark.c
    src/render/shader.c
    src/glad/glad.c
)

if(WIN32)
    list(APPEND SPARK_SOURCES src/platform/win32/win32.c)
elseif(UNIX AND NOT APPLE)
    list(APPEND SPARK_SOURCES src/platform/x11/x11.c)
    find_package(X11 REQUIRED)
else()
    list(APPEND SPARK_SOURCES src/platform/cocoa/cocoa.m)
endif()

find_package(OpenGL REQUIRED)

add_library(spark STATIC ${SPARK_SOURCES})

target_include_directories(spark PUBLIC include)

target_link_libraries(spark PUBLIC
    base
    OpenGL::GL
    $<$<BOOL:${X11_FOUND}>:X11::X11>
)

target_compile_options(spark PRIVATE
    $<$<C_COMPILER_ID:GNU,Clang>:-Wall -Wextra>
    $<$<C_COMPILER_ID:MSVC>:/W4>
)
