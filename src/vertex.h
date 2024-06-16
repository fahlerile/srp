#pragma once

#include <stddef.h>
#include "type.h"

typedef struct SRPVertex SRPVertex;

typedef struct
{
	size_t nItems;
	Type type;
	size_t offsetBytes;
} SRPVertexVariable;

