set(
    SOURCES

    main.c
    mainUtils/mainUtils.c
    Renderer.c

    Face.c
    Texture.c
    Model.c
    Scene.c

    utils/NDC.c
    utils/compares.c
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
add_executable(${EXECUTABLE_NAME} ${SOURCES})
target_include_directories(${EXECUTABLE_NAME} PUBLIC .)

if(MSVC)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${EXECUTABLE_NAME} PRIVATE
                         -Wall -Wextra -Wpedantic -Wno-unused -std=c2x)
    if(UNIX AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_options(${EXECUTABLE_NAME} PRIVATE -fsanitize=address)
    endif()
endif()

