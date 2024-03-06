set(
    SOURCES

    main.c
    Context.c
    Renderer.c
    VertexBuffer.c
    Uniforms.c
    draw.c
    Shaders.c
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})
target_include_directories(${EXECUTABLE_NAME} PUBLIC .)
target_compile_options(${EXECUTABLE_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Wno-unused -std=c2x -march=native
)

if (ENABLE_ASAN)
    target_link_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=address)
endif()

if (ENABLE_GPROF)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE -pg)
    target_link_options(${EXECUTABLE_NAME} PRIVATE -pg)
endif()

