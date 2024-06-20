// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#include "message_callback_p.h"
#include "context.h"

void srpNewContext(SRPContext* pContext)
{
	pContext->messageCallback = NULL;
	pContext->messageCallbackUserParameter = NULL;
	pContext->attributeInterpolation = SRP_ATTR_INTERPOLATION_PERSPECTIVE;
}

void srpContextSetP(SRPContextParameter contextParameter, const void* data)
{
	switch (contextParameter)
	{
	case CTX_PARAM_MESSAGE_CALLBACK:
		srpContext.messageCallback = data;
		return;
	case CTX_PARAM_MESSAGE_CALLBACK_USER_PARAMETER:
		/** @todo Is this cast OK? */
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

void srpContextSetI(SRPContextParameter contextParameter, int data)
{
	switch (contextParameter)
	{
	case CTX_PARAM_ATTRIBUTE_INTERPOLATION_TYPE:
		srpContext.attributeInterpolation = data;
	default:
		srpMessageCallbackHelper(
			MESSAGE_ERROR, MESSAGE_SEVERITY_HIGH, __func__,
			"Unknown type (%i)", contextParameter
		);
		return;
	}
}

