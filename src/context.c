// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#define SRP_SOURCE

#include "message_callback.h"
#include "context.h"

void srpNewContext(SRPContext* pContext)
{
	pContext->messageCallback = NULL;
	pContext->messageCallbackUserParameter = NULL;
}

void srpContextSetP(SRPContextParameter contextParameter, const void* data)
{
	switch (contextParameter)
	{
	case CTX_PARAM_MESSAGE_CALLBACK:
		srpContext.messageCallback = data;
		return;
	case CTX_PARAM_MESSAGE_CALLBACK_USER_PARAMETER:
		// TODO: is this cast OK?
		srpContext.messageCallbackUserParameter = (void*) data;
		return;
	default:
		srpMessageCallbackHelper(
			MESSAGE_ERROR, MESSAGE_SEVERITY_HIGH, __func__,
			"Unknown type (%i)", contextParameter
		);
		return;
	}
}

