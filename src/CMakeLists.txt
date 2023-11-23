set(
  SOURCES

  main.c
  MainUtils/MainUtils.c
  Renderer.c

  Triangle.c
  Texture.c
  Model.c
  Scene.c

  Utils/MemoryUtils.c
  Utils/NDC.c
  Utils/Matrix.c
  Utils/Color.c
  Utils/DynamicArray.c
  Utils/compares.c
  Utils/Vectors/Vector2.c
  Utils/Vectors/Vector3.c
  Utils/Vectors/Vector4.c
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
