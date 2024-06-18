#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct SRPFramebuffer
{
	size_t width;
	size_t height;
	size_t size;
	uint32_t* color;
	double* depth;
} SRPFramebuffer;

SRPFramebuffer* srpNewFramebuffer(size_t width, size_t height);
void srpFreeFramebuffer(SRPFramebuffer* this);

void framebufferClear(const SRPFramebuffer* this);

#ifdef SRP_SOURCE

// The `const` qualifier is completely legal: only the buffer pointed to by
// `framebuffer->color` is modified, not the structure itself!
void framebufferDrawPixel(
	const SRPFramebuffer* this, size_t x, size_t y, double depth,
	uint32_t color
);

void framebufferNDCToScreenSpace(
	const SRPFramebuffer* this, const double* NDC, double* SS
);

#endif  // ifdef SRP_SOURCE

