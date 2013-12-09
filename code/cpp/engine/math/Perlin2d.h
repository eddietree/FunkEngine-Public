
#pragma once

#include <math.h>
#include <gm/gmBindHeader.h>
#include <common/HandledObj.h>

namespace funk
{
	class Perlin2d : public HandledObj<Perlin2d>
	{
	public:

		Perlin2d( float persistence = 0.5f, float freq = 2.0, int nOctaves = 8 );
		float Get( float x, float y ) const;
		float GetNormalized( float x, float y ) const;

		GM_BIND_TYPEID(Perlin2d);

	private:

		// member vars
		float	m_persistence;
		float	m_freq;
		int		m_octaves;

		float InterpNoise( float x, float y ) const;
		
		inline float Noise( int x, int y ) const
		{
			int n = x + y * 57;
			n = ( n << 13 ) ^ n;
			return ( 1.f - ( ( n * ( n * n * 15731 + 789221 ) + 1376312589 ) & 0x7fffffff ) / 1073741824.0f );    
		}
		
		inline float CosineInterp( float a, float b, float x ) const
		{
			const float ft = x * 3.1415927f;
			float f = (1.0f - cosf(ft)) * 0.5f;
			return  a * ( 1 - f ) + b * f;
		}
		
		inline float SmoothNoise( float x, float y ) const
		{
			float corners = ( Noise((int)(x-1), (int)(y-1))+Noise((int)(x+1), (int)(y-1))+Noise((int)(x-1), (int)(y+1))+Noise((int)(x+1), (int)(y+1)) ) * 0.0625f;
			float sides   = ( Noise((int)(x-1), (int)(y))  +Noise((int)(x+1), (int)(y))  +Noise((int)(x), (int)(y-1))  +Noise((int)(x), (int)(y+1)) ) * 0.125f;
			float center  =  Noise((int)(x), (int)(y)) * 0.25f;
			return corners + sides + center;
		}
	};

	GM_BIND_DECL(Perlin2d);
}
