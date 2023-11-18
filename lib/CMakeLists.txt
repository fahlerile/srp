# Find .dll.a
find_library(
  SDL2_DLL_A
  SDL2.dll
  HINTS SDL2/lib
  REQUIRED
)

# Link to .dll.a
target_link_libraries(
  ${EXECUTABLE_NAME} PRIVATE
  ${SDL2_DLL_A}
)

target_include_directories(${EXECUTABLE_NAME} PUBLIC SDL2/include)
file(COPY SDL2/bin/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR}/bin/)  # copy .dll to bin directory

target_include_directories(${EXECUTABLE_NAME} PUBLIC include)
