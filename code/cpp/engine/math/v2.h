
#pragma once

#include <assert.h>
#include <math.h>
#include <float.h>
#include "Util.h"
#include <common/Debug.h>

#include <gm/gmVector.h>

namespace funk
{
class v2
{
public:
	union
	{
		struct { float x, y; };
		gmVec2 gmv2;
	};

	v2();
	explicit v2( float v );
	v2( const gmVec2 &_gmv2 ) { gmv2 = _gmv2; }
	v2( float _x, float _y );

	float&			operator[]( size_t i )				{assert( i < 2 ); return (&x)[i];}
	const float&	operator[]( size_t i ) const		{assert( i < 2 ); return (&x)[i];}

	v2&		operator+=( const v2& o );
	v2&		operator-=( const v2& o );
	v2&		operator*=( const v2& o );
	v2&		operator/=( const v2& o );
	v2&		operator*=( float s );
	v2&		operator/=( float s );
	
	v2		operator*( const v2& o ) const;
	v2		operator/( const v2& o ) const;
	v2		operator+( const v2& o ) const;
	v2		operator-( const v2& o ) const;
	v2		operator-() const;
	v2		operator*( float s ) const;
	v2		operator/( float s ) const;

	bool	operator==( const v2& o ) const;
	bool	operator!=( const v2& o ) const;
};

bool checkV2( const v2& );

float	length( const v2& v );
float	lengthSqr( const v2& v );
float	dot( const v2& a, const v2& b );
float	distance( const v2& p0, const v2& p1 );

v2	operator*( float s, const v2& v );

v2	normalize( const v2& v );
v2	max( const v2& a, const v2& b );
v2	min( const v2& a, const v2& b );
v2	abs( const v2& v );
v2	lerp( const v2& x, const v2& y, float a );
v2	clamp( const v2& v, const v2& min, const v2& max );
v2	saturate( const v2& v );
v2	perpendicular( const v2& v );
v2  unitCircle( float angleRad );

v2 hermite( const v2& v0, const v2& tan0, const v2& v1, const v2& tan1, float t );

#include "v2.inl"
}
