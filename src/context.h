// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

/** @file
 *  SRPContext and related functions */

#pragma once

#include <stddef.h>
#include "message_callback.h"

/** @ingroup Context
 *  @{ */

/** Holds runtime settings. 
 *	It always needs to be declared as `SRPContext srpContext` in user programs 
 *	and initialized with srpNewContext() */
typedef struct SRPContext
{
	SRPMessageCallbackType messageCallback;
	void* messageCallbackUserParameter;
} SRPContext;

/** An enumeration to hold possible arguments for `srpContextSet...` */
typedef enum SRPContextParameter
{
	CTX_PARAM_MESSAGE_CALLBACK,
	CTX_PARAM_MESSAGE_CALLBACK_USER_PARAMETER
} SRPContextParameter;

/** Initialize the context
 *  @param[in] pContext The pointer to context */
void srpNewContext(SRPContext* pContext);

/** Set a pointer parameter in the context
 *  @param[in] contextParameter The context parameter you want to modify
 *  @param[in] data The pointer you want to assign to specified context parameter */
void srpContextSetP(SRPContextParameter contextParameter, const void* data);

/** @} */  // defgroup Context

#ifdef SRP_SOURCE
extern SRPContext srpContext;
#endif

