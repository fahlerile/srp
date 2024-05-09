set(
    SOURCES
    Renderer.c
    buffer.c
    shaders.c
    triangle.c
)

add_library(rasterizer STATIC ${SOURCES})
target_link_libraries(
    rasterizer
    PRIVATE
    SDL2
    memoryUtils
    Vector
)
target_include_directories(
    rasterizer
    PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/c-codebase
)

if (ENABLE_ASAN)
    target_link_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=address)
endif()
if (ENABLE_UBSAN)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=undefined)
    target_link_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=undefined)
endif()

if (ENABLE_GPROF)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE -pg)
    target_link_options(${EXECUTABLE_NAME} PRIVATE -pg)
endif()

