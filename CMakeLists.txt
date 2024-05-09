cmake_minimum_required(VERSION 3.18.4)
project(rasterizer)

set(EXECUTABLE_NAME rasterizer)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory(src)
add_subdirectory(lib)

