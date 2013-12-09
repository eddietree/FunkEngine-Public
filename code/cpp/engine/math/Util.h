
#pragma once

#include <math.h>
#include <cstdlib>

namespace funk
{
#undef min
#undef max

	template< typename T >
	inline T lerp( T a, T b, float lerp )
	{
		return (T)(a + (b-a)*lerp);
	}

	template< typename T >
	inline T min( T a, T b )
	{
		return a < b ? a : b;
	}

	template< typename T >
	inline T max( T a, T b )
	{
		return a > b ? a : b;
	}

	template< typename T >
	inline T clamp( T val, T left, T right )
	{
		return min( max( val, left ), right );
	}

	template< typename T >
	inline T saturate( T val )
	{
		return clamp(val, (T)0, (T)1);
	}

	template< typename T >
	inline T sign( T val )
	{
		if (val < 0)
			return -1;
		if (val > 0)
			return 1;
		return 0;
	}

	template< typename T >
	inline T smoothstep( T a, T b, float alpha )
	{
		alpha = saturate( (alpha-a)/(b-a) );
		return alpha*alpha*(3.0f-2.0f*alpha);
	}

	// Interpolates between p1 and p2, using t:[0,1]
	template< typename T >
	inline T CubicInterpolate( T t0, T p0, T p1, T t1, float t )
	{
		T a = -0.5f * t0 + 1.5f * p0 - 1.5f * p1 + 0.5f * t1;
		T b = t0 - 2.5f * p0 + 2.0f * p1 - 0.5f * t1;
		T c = -0.5f * t0 + 0.5f * p1;
		T d = p0;

		return a*t*t*t + b*t*t + c*t + d;
	}

	template< typename T >
	inline T CosInterpolate( T p0, T p1, float t )
	{
		float angle = t * 3.14159f;
		float prc = ( 1.0f - cosf(angle) ) * 0.5f;
		return p0 * (1.0f - prc) + p1 * prc;
	}

	template< typename T >
	inline T randbetween( T min, T max )
	{
		float randNormalized = (float)rand() / RAND_MAX;
		return min + T((max-min) * randNormalized);
	}

	template<>
	inline int randbetween( int min, int max )
	{
		return rand() % (max-min) + min;
	}

	inline float fastSqrtf(float number) 
	{
		long i;
		float x, y;
		const float f = 1.5F;

		x = number * 0.5F;
		y  = number;
		i  = * ( long * ) &y;
		i  = 0x5f3759df - ( i >> 1 );
		y  = * ( float * ) &i;
		y  = y * ( f - ( x * y * y ) );
		y  = y * ( f - ( x * y * y ) );
		return number * y;
	}

	inline float wrapf(float val, float minVal, float maxVal)
	{
		val -= minVal;
		
		const float delta = maxVal - minVal;
		if ( delta < 0.0001f ) return val;

		return val - (delta* floorf(val/delta)) + minVal;
	}

	inline int wrapi(int val, int minVal, int maxVal)
	{
		int range_size = maxVal - minVal + 1;
		if (val < minVal) val += range_size * ((minVal - val) / range_size + 1);
		return minVal + (val - minVal) % range_size;
	}

    // quake fast inverse square root, faster than using regular sqrt(), slower than using SSE rsqrt
    // for timings see: http://assemblyrequired.crashworks.org/2009/10/16/timing-square-root/
    inline float invsqrt(float number)
    {
        long i;
        float x2, y;
        const float threehalfs = 1.5F;
 
        x2 = number * 0.5F;
        y  = number;
        i  = * ( long * ) &y;                       // evil floating point bit level hacking
        i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
        y  = * ( float * ) &i;
        y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
 
        return y;
    }
}
