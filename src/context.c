#define SRP_SOURCE

#include "message_callback.h"
#include "context.h"

void srpNewContext(SRPContext* context)
{
	context->messageCallback = NULL;
	context->messageCallbackUserParameter = NULL;
}

void srpContextSetP(SRPContextParameter contextParameter, void** data)
{
	switch (contextParameter)
	{
	case CTX_PARAM_MESSAGE_CALLBACK:
		srpContext.messageCallback = *data;
		return;
	case CTX_PARAM_MESSAGE_CALLBACK_USER_PARAMETER:
		srpContext.messageCallbackUserParameter = *data;
		return;
	default:
		messageCallback(
			MESSAGE_ERROR, MESSAGE_SEVERITY_HIGH, __func__,
			"Unknown type (%i)", contextParameter
		);
		return;
	}
}

