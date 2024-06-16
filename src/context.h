#pragma once

#include <stddef.h>
#include "message_callback.h"

typedef struct SRPContext
{
	SRPMessageCallbackType messageCallback;
	void* messageCallbackUserParameter;
} SRPContext;

typedef enum SRPContextParameter
{
	CTX_PARAM_MESSAGE_CALLBACK,
	CTX_PARAM_MESSAGE_CALLBACK_USER_PARAMETER
} SRPContextParameter;

void srpNewContext(SRPContext* context);
void srpContextSetP(SRPContextParameter contextParameter, void* data);

extern SRPContext srpContext;

