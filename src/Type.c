#include <stdio.h>
#include "Type.h"

size_t SIZEOF_TYPE(Type type)
{
	switch (type)
	{
		case TYPE_UINT8:
			return sizeof(uint8_t);
		case TYPE_UINT16:
			return sizeof(uint16_t);
		case TYPE_UINT32:
			return sizeof(uint32_t);
		case TYPE_UINT64:
			return sizeof(uint64_t);
		case TYPE_FLOAT:
			return sizeof(float);
		case TYPE_DOUBLE:
			return sizeof(double);
		default:
			fprintf(stderr, "Unknown type (%i) in %s", type, __func__);
			return 0;
	}
}

