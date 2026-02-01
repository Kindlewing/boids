cmake_minimum_required(VERSION 3.20)

if(WIN32)
    set(BASE_PLATFORM_SRC src/arena_win32.c)
else()
    set(BASE_PLATFORM_SRC src/arena_linux.c)
endif()

add_library(base STATIC
    ${BASE_PLATFORM_SRC}
    src/string8.c
    src/base_math.c
	src/profile.c
)

target_include_directories(base PUBLIC include)

target_compile_options(base PRIVATE
    $<$<C_COMPILER_ID:GNU,Clang>:-Wall -Wextra>
    $<$<C_COMPILER_ID:MSVC>:/W4>
)
