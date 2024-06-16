#pragma once


#if defined(SRP_MALLOC) && defined(SRP_FREE) && defined(SRP_REALLOC)
	// ok
#elif !defined(SRP_MALLOC) && !defined(SRP_FREE) && !defined(SRP_REALLOC)
	#include "alloc.h"
	#define SRP_MALLOC(s) srpMalloc(s)
	#define SRP_REALLOC(p, s) srpRealloc(p, s)
	#define SRP_FREE(p) srpFree(p)
#else
	#error "Must define all or none of SRP_MALLOC, SRP_FREE, and SRP_REALLOC"
#endif

#ifndef SRP_FORCEINLINE
	#if defined(__clang__)   // clang
		#define SRP_FORCEINLINE inline
	#elif defined(__GNUC__)  // gcc
		#define SRP_FORCEINLINE __attribute__((always_inline)) inline
	#elif defined(_MSC_VER)  // msvc
		#define SRP_FORCEINLINE __forceinline
	#else
		#define SRP_FORCEINLINE inline
		#warning \
			"Unrecognized compiler - setting `SRP_FORCEINLINE` to just `inline` \
			(it is not forced anymore!)"
	#endif
#endif

