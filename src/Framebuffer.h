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

void framebufferClear(SRPFramebuffer* this);

#ifdef SRP_SOURCE

void framebufferDrawPixel(
	SRPFramebuffer* this, size_t x, size_t y, double depth, uint32_t color
);

void framebufferNDCToScreenSpace(SRPFramebuffer* this, double* NDC, double* SS);

uint32_t* framebufferGetPixelPointer(SRPFramebuffer* this, size_t x, size_t y);
double* framebufferGetDepthPointer(SRPFramebuffer* this, size_t x, size_t y);

#endif  // ifdef SRP_SOURCE

