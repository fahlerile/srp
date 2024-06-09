set(
	SOURCES
	buffer.c
	triangle.c
	Framebuffer.c
	Type.c
	Texture.c
	stb_image.c
)

add_library(rasterizer STATIC ${SOURCES})
target_link_libraries(
	rasterizer
	PRIVATE
	SDL2
	memoryUtils
	Vector
)

