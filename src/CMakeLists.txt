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
	alloc.c
)

add_library(srp STATIC ${SOURCES})

