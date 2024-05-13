#pragma once
#include <stddef.h>
#include <stdint.h>

typedef struct
{
	size_t width;
	size_t height;
	size_t size;
	uint32_t* color;
	double* depth;
} Framebuffer;

Framebuffer* newFramebuffer(size_t width, size_t height);
void freeFramebuffer(Framebuffer* this);

void framebufferDrawPixel(
	Framebuffer* this, size_t x, size_t y, double depth, uint32_t color
);

void framebufferNDCToScreenSpace(Framebuffer* this, double* NDC, double* SS);

void framebufferClear(Framebuffer* this);
uint32_t* framebufferGetPixelPointer(Framebuffer* this, size_t x, size_t y);
double* framebufferGetDepthPointer(Framebuffer* this, size_t x, size_t y);

