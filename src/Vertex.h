#pragma once
#include <stddef.h>
#include "Type.h"

// Incomplete "filler" type
// TODO: what if there are multiple vertex types?
typedef struct Vertex Vertex;

typedef struct
{
	size_t nItems;
	Type type;
	size_t offsetBytes;
} VertexAttribute;

