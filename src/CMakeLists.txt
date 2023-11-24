set(
  SOURCES

  main.c
  mainUtils/mainUtils.c
  Renderer.c

  Triangle.c
  Texture.c
  Model.c
  Scene.c

  utils/MemoryUtils.c
  utils/NDC.c
  utils/Matrix.c
  utils/Color.c
  utils/DynamicArray.c
  utils/compares.c
  utils/Vectors/Vector2.c
  utils/Vectors/Vector3.c
  utils/Vectors/Vector4.c
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

add_executable(${EXECUTABLE_NAME} ${SOURCES})

if(MSVC)
  target_compile_options(${EXECUTABLE_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${EXECUTABLE_NAME} PRIVATE
                         -Wall -Wextra -Wpedantic -Wno-unused -std=c2x)  # -march=native)
endif()

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
