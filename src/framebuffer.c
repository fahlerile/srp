#define SRP_SOURCE

#include <stdio.h>
#include <assert.h>
#include "framebuffer.h"
#include "message_callback.h"
#include "defines.h"

SRPFramebuffer* srpNewFramebuffer(size_t width, size_t height)
{
	SRPFramebuffer* this = SRP_MALLOC(sizeof(SRPFramebuffer));

	this->width = width;
	this->height = height;
	this->size = width * height;
	this->color = SRP_MALLOC(sizeof(uint32_t) * this->size);
	this->depth = SRP_MALLOC(sizeof(double) * this->size);

	return this;
}

void srpFreeFramebuffer(SRPFramebuffer* this)
{
	SRP_FREE(this->color);
	SRP_FREE(this->depth);
	SRP_FREE(this);
}

void framebufferDrawPixel(
	SRPFramebuffer* this, size_t x, size_t y, double depth, uint32_t color
)
{
	if (1 < depth || depth < -1)
		srpMessageCallbackHelper(
			MESSAGE_ERROR, MESSAGE_SEVERITY_HIGH, __func__,
			"Depth value is not inside [-1, 1] interval; depth=%lf", depth
		);

	double* pDepth = framebufferGetDepthPointer(this, x, y);
	if (depth < *pDepth)
		return;

	*framebufferGetPixelPointer(this, x, y) = color;
	*pDepth = depth;
}

void framebufferNDCToScreenSpace(SRPFramebuffer* this, double* NDC, double* SS)
{
	SS[0] =  (((double) this->width  - 1) / 2) * (NDC[0] + 1);
	SS[1] = -(((double) this->height - 1) / 2) * (NDC[1] - 1),
	SS[2] = NDC[2];
}

void framebufferClear(SRPFramebuffer *this)
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

uint32_t* framebufferGetPixelPointer(SRPFramebuffer* this, size_t x, size_t y)
{
	return this->color + (y * this->width + x);
}

double* framebufferGetDepthPointer(SRPFramebuffer* this, size_t x, size_t y)
{
	return this->depth + (y * this->width + x);
}

