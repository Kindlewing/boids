add_library(spark STATIC
    src/spark.c
    src/render/shader.c
    src/platform/x11/x11.c
    src/glad/glad.c 
)

# Public headers
target_include_directories(spark PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Spark depends on system libraries
target_link_libraries(spark PUBLIC base GL X11)
