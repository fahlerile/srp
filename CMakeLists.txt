cmake_minimum_required(VERSION 3.18.4)
project(rasterizer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_compile_options(
    -Wall -Wextra -Wpedantic -Wno-unused -std=c2x -march=native -O3
)

if (ENABLE_ASAN)
    add_compile_options(-fsanitize=address)
    link_libraries(asan)
endif()
if (ENABLE_LSAN)
    add_compile_options(-fsanitize=leak)
    link_libraries(asan)
endif()
if (ENABLE_UBSAN)
    add_compile_options(-fsanitize=undefined)
    link_libraries(ubsan)
endif()
if (ENABLE_GPROF)
    add_compile_options(-pg)
    add_link_options(-pg)
endif()

add_subdirectory(src)
add_subdirectory(lib)
add_subdirectory(examples)

