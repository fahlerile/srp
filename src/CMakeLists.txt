set(
	SOURCES
	context.c
	buffer.c
	triangle.c
	framebuffer.c
	type.c
	texture.c
	stb_image.c
	vec.c
	mat.c
)

add_library(rasterizer STATIC ${SOURCES})

