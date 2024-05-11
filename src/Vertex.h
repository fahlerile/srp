#pragma once
#include <stddef.h>
#include "Type.h"

typedef struct Vertex Vertex;

typedef struct
{
	size_t nItems;
	Type type;
	size_t offsetBytes;
} VertexAttribute;

