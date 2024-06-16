#pragma once

#include <stdint.h>
#include <arpa/inet.h>

typedef struct SRPColor
{
	uint8_t r, g, b, a;
} SRPColor;

// Convert whatever the host endianness is to big endian (network endian)
#define SRP_COLOR_TO_UINT32_T(color) (htonl(*(uint32_t*) &color))

