#pragma once

#ifdef SRP_SOURCE

#include "framebuffer.h"
#include "shaders.h"
#include "vec.h"

void drawTriangle(
	SRPFramebuffer* fb, const SRPvsOutput vertices[3],
	const SRPShaderProgram* restrict sp, size_t primitiveID
);

#endif  // ifdef SRP_SOURCE

