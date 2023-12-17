# target_include_directories(Color PUBLIC . ${CMAKE_SOURCE_DIR}/src)
# target_include_directories(Vector PUBLIC . ${CMAKE_SOURCE_DIR}/src)
# target_include_directories(Matrix PUBLIC . ${CMAKE_SOURCE_DIR}/src)
# target_include_directories(DynamicArray PUBLIC . ${CMAKE_SOURCE_DIR}/src)
# target_include_directories(fileUtils PUBLIC . ${CMAKE_SOURCE_DIR}/src)
# target_include_directories(memoryUtils PUBLIC . ${CMAKE_SOURCE_DIR}/src)
# target_include_directories(stringUtils PUBLIC . ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(c-codebase)

target_link_libraries(
    ${EXECUTABLE_NAME}
    Color
    Vector
    Matrix
    DynamicArray
    fileUtils
    memoryUtils
    stringUtils
)

target_link_libraries(
    ${EXECUTABLE_NAME}
    SDL2 SDL2main
)

target_include_directories(
    ${EXECUTABLE_NAME} PUBLIC
    c-codebase
    include
)

