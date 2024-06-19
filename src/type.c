// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#define SRP_SOURCE

#include <stdio.h>
#include "message_callback.h"
#include "type.h"

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
		{
			srpMessageCallbackHelper(
				MESSAGE_ERROR, MESSAGE_SEVERITY_HIGH, __func__,
				"Unknown type (%i)", type
			);
			return 0;
		}
	}
}

