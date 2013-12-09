#include "Perlin2d.h"

#include <gm/gmBind.h>

namespace funk
{
Perlin2d::Perlin2d( float persistence, float freq, int nOctaves )
: m_persistence(persistence), m_freq(freq), m_octaves(nOctaves)
{;}

float Perlin2d::InterpNoise(float x, float y) const
{
	float integer_x_int    = floor(x);
	float fractional_x_int = fabs(x - integer_x_int);

	float integer_Y    = floor(y);
	float fractional_Y = fabs(y - integer_Y);

	float v1 = SmoothNoise(integer_x_int,     integer_Y);
	float v2 = SmoothNoise(integer_x_int + 1, integer_Y);
	float v3 = SmoothNoise(integer_x_int,     integer_Y + 1);
	float v4 = SmoothNoise(integer_x_int + 1, integer_Y + 1);

	float i1 = CosineInterp(v1 , v2, fractional_x_int);
	float i2 = CosineInterp(v3 , v4, fractional_x_int);

	return CosineInterp(i1 , i2 , fractional_Y);
}

float Perlin2d::Get( float x, float y ) const
{
	float total = 0.0f;

	for( int i = 0; i < m_octaves; ++i )
	{
		float frequency = pow(m_freq, (float)i);
		float amplitude = pow(m_persistence, (float)i);
		total = total + InterpNoise(x * frequency, y * frequency) * amplitude;
	}

	return total;
}

float Perlin2d::GetNormalized( float x, float y ) const
{
	const float value = Get(x, y);
	const float result = (value + 1.0f) * 0.5f;

	return result;
}

GM_REG_NAMESPACE(Perlin2d)
{
	GM_MEMFUNC_CONSTRUCTOR(Perlin2d)
	{
		int numParams = a_thread->GetNumParams();

		if ( numParams == 0 )
		{
			GM_PUSH_USER_HANDLED( Perlin2d, new Perlin2d );
		}
		else if ( numParams == 3 )
		{
			GM_CHECK_FLOAT_PARAM( persistence,	0 );
			GM_CHECK_FLOAT_PARAM( freq,			1 );
			GM_CHECK_INT_PARAM( octaves,		2 );

			GM_PUSH_USER_HANDLED( Perlin2d, new Perlin2d(persistence, freq, octaves ) );
		}

		return GM_OK;
	}

	GM_MEMFUNC_DECL(Get)
	{
		int numParams = a_thread->GetNumParams();

		float x,y;

		if ( numParams == 1 )
		{
			GM_CHECK_V2_PARAM( vec, 0 );
			x = vec.x;
			y = vec.y;
		}
		else if ( numParams == 2 )
		{
			GM_CHECK_FLOAT_PARAM( _x, 0 );
			GM_CHECK_FLOAT_PARAM( _y, 1 );

			x = _x;
			y = _y;
		}
		else
		{
			return GM_EXCEPTION;
		}

		GM_GET_THIS_PTR(Perlin2d, ptr);
		a_thread->PushFloat( ptr->Get(x,y) );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetNormalized)
	{
		int numParams = a_thread->GetNumParams();

		float x,y;

		if ( numParams == 1 )
		{
			GM_CHECK_V2_PARAM( vec, 0 );
			x = vec.x;
			y = vec.y;
		}
		else if ( numParams == 2 )
		{
			GM_CHECK_FLOAT_PARAM( _x, 0 );
			GM_CHECK_FLOAT_PARAM( _y, 1 );

			x = _x;
			y = _y;
		}
		else
		{
			return GM_EXCEPTION;
		}

		GM_GET_THIS_PTR(Perlin2d, ptr);
		a_thread->PushFloat( ptr->GetNormalized(x,y) );

		return GM_OK;
	}
}

GM_REG_MEM_BEGIN(Perlin2d)
GM_REG_MEMFUNC( Perlin2d, Get )
GM_REG_MEMFUNC( Perlin2d, GetNormalized )
GM_REG_HANDLED_DESTRUCTORS(Perlin2d)
GM_REG_MEM_END()
GM_BIND_DEFINE(Perlin2d)
}