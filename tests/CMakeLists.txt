add_executable(DynamicArrayTest DynamicArrayTest.c)
add_executable(VectorsTest VectorsTest.c)
add_executable(MatrixTest MatrixTest.c)
add_executable(fileUtilsTest fileUtilsTest.c)
add_executable(stringUtilsTest stringUtilsTest.c)

target_link_libraries(DynamicArrayTest DynamicArray memoryUtils)
# target_link_libraries(VectorsTest)
# target_link_libraries(MatrixTest)
# target_link_libraries(DynamicArrayTest DynamicArray memoryUtils)
# target_link_libraries(DynamicArrayTest DynamicArray memoryUtils)

if(NOT WIN32)
# target_link_libraries()
endif()

file(COPY res/ DESTINATION ${CMAKE_BINARY_DIR}/tests/res)

