#include <assert.h>
#include <stdint.h>
#include "Framebuffer.h"
#include "memoryUtils/memoryUtils.h"

Framebuffer* newFramebuffer(size_t width, size_t height)
{
	Framebuffer* this = xmalloc(sizeof(Framebuffer));

	this->width = width;
	this->height = height;
	this->size = width * height;
	this->color = xmalloc(sizeof(uint32_t) * this->size);
	this->depth = xmalloc(sizeof(double) * this->size);

	return this;
}

void freeFramebuffer(Framebuffer* this)
{
	xfree(this->color);
	xfree(this->depth);
	xfree(this);
}

void framebufferDrawPixel(
	Framebuffer* this, size_t x, size_t y, double depth, uint32_t color
)
{
	assert(1 >= depth);
	assert(depth >= -1);

	double* pDepth = framebufferGetDepthPointer(this, x, y);
	if (depth < *pDepth)
		return;

	*framebufferGetPixelPointer(this, x, y) = color;
	*pDepth = depth;
}

void framebufferNDCToScreenSpace(Framebuffer* this, double* NDC, double* SS)
{
	SS[0] =  (((double) this->width  - 1) / 2) * (NDC[0] + 1);
	SS[1] = -(((double) this->height - 1) / 2) * (NDC[1] - 1),
	SS[2] = NDC[2];
}

void framebufferClear(Framebuffer *this)
{
	for (size_t y = 0; y < this->height; y++)
	{
		for (size_t x = 0; x < this->width; x++)
		{
			*framebufferGetPixelPointer(this, x, y) = 0x000000FF;
			*framebufferGetDepthPointer(this, x, y) = -1;
		}
	}
}

uint32_t* framebufferGetPixelPointer(Framebuffer* this, size_t x, size_t y)
{
	return this->color + (y * this->width + x);
}

double* framebufferGetDepthPointer(Framebuffer* this, size_t x, size_t y)
{
	return this->depth + (y * this->width + x);
}

