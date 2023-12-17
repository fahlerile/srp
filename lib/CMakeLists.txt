add_library(Color STATIC Color/Color.c)
add_library(Vector STATIC Vector/Vector2.c Vector/Vector3.c Vector/Vector4.c)
add_library(Matrix STATIC Matrix/Matrix.c)
add_library(DynamicArray STATIC DynamicArray/DynamicArray.c)
add_library(fileUtils STATIC fileUtils/fileUtils.c)
add_library(memoryUtils STATIC memoryUtils/memoryUtils.c)
add_library(stringUtils STATIC stringUtils/stringUtils.c)

target_link_libraries(Matrix Vector)

target_include_directories(Color PUBLIC . ${CMAKE_SOURCE_DIR}/src)
target_include_directories(Vector PUBLIC . ${CMAKE_SOURCE_DIR}/src)
target_include_directories(Matrix PUBLIC . ${CMAKE_SOURCE_DIR}/src)
target_include_directories(DynamicArray PUBLIC . ${CMAKE_SOURCE_DIR}/src)
target_include_directories(fileUtils PUBLIC . ${CMAKE_SOURCE_DIR}/src)
target_include_directories(memoryUtils PUBLIC . ${CMAKE_SOURCE_DIR}/src)
target_include_directories(stringUtils PUBLIC . ${CMAKE_SOURCE_DIR}/src)

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
    include
)

