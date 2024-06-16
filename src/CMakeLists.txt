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

