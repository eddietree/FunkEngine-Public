
#pragma once

#include <system/Config.h>

#if PLATFORM_MOBILE

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#else
	#if defined(__APPLE__)
		// Not OpenGL ES
		#include <OpenAL/al.h>
		#include <OpenAL/alc.h>
	#else
		// window sopengl
		#include <al/al.h>
		#include <al/alc.h>
		#include <al/efx.h>
	#endif
#endif // PLATFORM_MOBILE
