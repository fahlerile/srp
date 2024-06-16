#pragma once

#include <stdint.h>
#include <arpa/inet.h>

typedef struct Color
{
	uint8_t r, g, b, a;
} Color;

// Convert whatever the host endianness is to big endian (network endian)
#define COLOR_TO_UINT32_T(color) (htonl(*(uint32_t*) &color))

