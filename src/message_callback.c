// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#define SRP_SOURCE

#include <stdio.h>
#include <stdarg.h>
#include "context.h"
#include "message_callback.h"

#define MAX_CHARS_IN_MESSAGE 1024

void srpMessageCallbackHelper(
	SRPMessageType type, SRPMessageSeverity severity,
	const char* sourceFunction, const char* format, ...
)
{
	if (srpContext.messageCallback != NULL)
	{
		char string[MAX_CHARS_IN_MESSAGE];
		va_list variadic;
		va_start(variadic, format);
		vsnprintf(string, MAX_CHARS_IN_MESSAGE, format, variadic);

		srpContext.messageCallback(
			type, severity, sourceFunction, string,
			srpContext.messageCallbackUserParameter
		);
	}
}

