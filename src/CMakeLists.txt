set(
	SOURCES
	context.c
	buffer.c
	triangle.c
	Framebuffer.c
	Type.c
	Texture.c
	stb_image.c
	vec.c
	mat.c
)

add_library(rasterizer STATIC ${SOURCES})

