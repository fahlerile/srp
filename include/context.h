// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

/** @file
 *  SRPContext and related functions */

#pragma once

#include <stddef.h>
#include "message_callback.h"

/** @ingroup Context
 *  @{ */

/** Available attribute interpolation types */
typedef enum SRPAttributeInterpolationType
{
	SRP_ATTR_INTERPOLATION_PERSPECTIVE,
	SRP_ATTR_INTERPOLATION_AFFINE
} SRPAttributeInterpolationType;

/** Holds runtime settings. This always needs to be declared as `SRPContext
 *  srpContext` in user programs and initialized with srpNewContext() */
typedef struct SRPContext
{
	SRPMessageCallbackType messageCallback;
	void* messageCallbackUserParameter;
	SRPAttributeInterpolationType attributeInterpolation;
} SRPContext;

/** Possible arguments to `srpContextSet...` */
typedef enum SRPContextParameter
{
	CTX_PARAM_MESSAGE_CALLBACK,
	CTX_PARAM_MESSAGE_CALLBACK_USER_PARAMETER,
	CTX_PARAM_ATTRIBUTE_INTERPOLATION_TYPE
} SRPContextParameter;

/** Initialize the context
 *  @param[in] pContext The pointer to context */
void srpNewContext(SRPContext* pContext);

/** Set a pointer parameter in the context
 *  @param[in] contextParameter The context parameter you want to modify
 *  @param[in] data The pointer you want to assign to specified context parameter */
void srpContextSetP(SRPContextParameter contextParameter, const void* data);

/** Set an integer/enum parameter in the context
 *  @param[in] contextParameter The context parameter you want to modify
 *  @param[in] data The value you want to assign to specified context parameter */
void srpContextSetI(SRPContextParameter contextParameter, int data);

/** @} */  // defgroup Context

extern SRPContext srpContext;

