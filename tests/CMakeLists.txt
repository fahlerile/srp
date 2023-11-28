add_executable(DynamicArrayTest
    DynamicArrayTest.c
    ${CMAKE_SOURCE_DIR}/src/utils/DynamicArray.c
    ${CMAKE_SOURCE_DIR}/src/utils/memoryUtils.c
)
add_executable(VectorsTest
    VectorsTest.c
    ${CMAKE_SOURCE_DIR}/src/utils/Vectors/Vector2.c
    ${CMAKE_SOURCE_DIR}/src/utils/Vectors/Vector3.c
    ${CMAKE_SOURCE_DIR}/src/utils/Vectors/Vector4.c
    ${CMAKE_SOURCE_DIR}/src/utils/compares.c
)
add_executable(MatrixTest
    MatrixTest.c
    ${CMAKE_SOURCE_DIR}/src/utils/Matrix.c
    ${CMAKE_SOURCE_DIR}/src/utils/Vectors/Vector3.c
    ${CMAKE_SOURCE_DIR}/src/utils/Vectors/Vector4.c
)
add_executable(fileUtilsTest
    fileUtilsTest.c
    ${CMAKE_SOURCE_DIR}/src/utils/fileUtils.c
    ${CMAKE_SOURCE_DIR}/src/utils/memoryUtils.c
)

set(testNames
    DynamicArrayTest
    VectorsTest
    MatrixTest
    fileUtilsTest
)

file(COPY res/ DESTINATION ${CMAKE_BINARY_DIR}/tests/res)

foreach(testName ${testNames})
    add_test(NAME ${testName} COMMAND ${testName})
    target_include_directories(${testName} PUBLIC ${CMAKE_SOURCE_DIR}/src)
    if(UNIX)
        target_link_libraries(${testName} m)
    endif()
endforeach(testName ${testNames})
