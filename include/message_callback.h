// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

/** @file
 *  Message callback declaration and helper functions */

#pragma once

/** @ingroup Context
 *  @{ */

/** An enumeration to represent the message type (error, warning, etc.)
 *  @see `SRPMessageCallbackType` */
typedef enum MessageType
{
	MESSAGE_ERROR,
	MESSAGE_WARNING
} SRPMessageType;

/** An enumeration to represent the message severity
 *  @see `SRPMessageCallbackType` */
typedef enum MessageSeverity
{
	MESSAGE_SEVERITY_LOW,
	MESSAGE_SEVERITY_MODERATE,
	MESSAGE_SEVERITY_HIGH
} SRPMessageSeverity;

/** A function of this type may be defined by user to handle the messages
 *  generated in the library implementation. @see `SRPContext` */
typedef void (*SRPMessageCallbackType)(
	SRPMessageType type, SRPMessageSeverity severity, const char* sourceFunction,
	const char* message, void* userParameter
);

/** @} */  // ingroup Context

