set(
	SOURCES
	buffer.c
	shaders.c
	triangle.c
	Framebuffer.c
	Type.c
)

add_library(rasterizer STATIC ${SOURCES})
target_link_libraries(
	rasterizer
	PRIVATE
	SDL2
	memoryUtils
	Vector
)
target_include_directories(
	rasterizer
	PRIVATE
	${CMAKE_SOURCE_DIR}/lib/c-codebase
)

