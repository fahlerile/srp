#pragma once

typedef enum MessageType
{
	MESSAGE_ERROR,
	MESSAGE_WARNING
} SRPMessageType;

typedef enum MessageSeverity
{
	MESSAGE_SEVERITY_LOW,
	MESSAGE_SEVERITY_MODERATE,
	MESSAGE_SEVERITY_HIGH
} SRPMessageSeverity;

typedef void (*SRPMessageCallbackType)(
	SRPMessageType type, SRPMessageSeverity severity, const char* sourceFunction,
	const char* message, void* userParameter
);

#ifdef SRP_SOURCE

void messageCallback(
	SRPMessageType type, SRPMessageSeverity severity,
	const char* sourceFunction, const char* message, ...
);

#endif  // ifdef SRP_SOURCE

