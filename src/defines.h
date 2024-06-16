#pragma once

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

