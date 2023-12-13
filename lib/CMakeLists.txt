if(WIN32)
  find_library(
    SDL2_DLL_A
    SDL2.dll
    HINTS SDL2/lib
    REQUIRED
  )
  target_link_libraries(
    ${EXECUTABLE_NAME}
    ${SDL2_DLL_A}
  )
  file(COPY SDL2/bin/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR}/bin/)
else()
  target_link_libraries(
    ${EXECUTABLE_NAME}
    SDL2
    m
  )
endif(WIN32)

target_include_directories(${EXECUTABLE_NAME} PUBLIC
  SDL2/include
  include
)
