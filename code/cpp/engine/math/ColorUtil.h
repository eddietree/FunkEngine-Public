
#pragma once

#include "v3.h"

#include <stdint.h>
#include <system/Config.h>

namespace funk
{
	v3 HSVtoRGB( v3 HSV ); // Hue in radians
	v3 RGBtoHSV( v3 RGB ); // Hue in radians

	inline uint32_t MakeColorUint32( float a_red, float a_green, float a_blue, float a_alpha )
	{
		uint8_t r = uint8_t(255*a_red);
		uint8_t g = uint8_t(255*a_green);
		uint8_t b = uint8_t(255*a_blue);
		uint8_t a = uint8_t(255*a_alpha);
		
		#if defined(IS_LITTLE_ENDIAN)
		uint32_t color = (a<<24) | (b<<16) | (g<<8) | (r);
		#else
		uint32_t color = (r<<24) | (g<<16) | (b<<8) | (a);
		#endif

		return color;
	}
}
