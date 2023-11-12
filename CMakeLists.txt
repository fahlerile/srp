cmake_minimum_required(VERSION 3.18.4)
project(rasterizer)

set(EXECUTABLE_NAME rasterizer)

add_subdirectory(src)
add_subdirectory(lib)

file(COPY res/ DESTINATION ${CMAKE_BINARY_DIR}/bin/)
