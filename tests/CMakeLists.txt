add_executable(DynamicArrayTest
    DynamicArrayTest.c
    ${CMAKE_SOURCE_DIR}/src/utils/DynamicArray.c
    ${CMAKE_SOURCE_DIR}/src/utils/MemoryUtils.c
)

set(testNames
    DynamicArrayTest
    # MatrixTest
    # VectorsTest
)

foreach(testName ${testNames})
    add_test(NAME ${testName} COMMAND ${testName})
    target_include_directories(${testName} PUBLIC ${CMAKE_SOURCE_DIR}/src)
endforeach(testName ${testNames})
