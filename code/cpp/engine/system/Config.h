#ifndef _INCLUDE_CONFIG_H
#define _INCLUDE_CONFIG_H

#define IS_LITTLE_ENDIAN (1 == *(unsigned char *)&(const int){1})

#if defined(__APPLE__)
	#include "TargetConditionals.h"

	#if TARGET_OS_IPHONE
		// iOS
		#define PLATFORM_MOBILE 1
		#define GL_ES 1
	#else
		// Mac OSX
		#define PLATFORM_MOBILE 0
		#define GL_ES 0
	#endif // TARGET_OS_IPHONE

#else

	// microsoft windows stuff?
	#define PLATFORM_MOBILE 0
	#define GL_ES 0

	// maybe android stuff?

#endif // __APPLE__

#endif // _INCLUDE_CONFIG_H