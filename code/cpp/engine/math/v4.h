
#pragma once

#include <assert.h>
#include <math.h>
#include <float.h>

#include "Util.h"
#include "v2.h"
#include "v3.h"

namespace funk
{
class v4
{
public:

	union
	{
		struct { float x, y, z, w; };
	};

	v4();
	explicit v4( float v );
	v4( float _x, float _y, float _z, float _w );
	v4( const v3 vec );
	v4( const v3 vec, float _w );

	float&			operator[]( size_t i )				{assert( i < 4 ); return (&x)[i];}
	const float&	operator[]( size_t i ) const		{assert( i < 4 ); return (&x)[i];}

	v4&		operator+=( const v4& o );
	v4&		operator-=( const v4& o );
	v4&		operator*=( const v4& o );
	v4&		operator/=( const v4& o );
	v4&		operator*=( float s );
	v4&		operator/=( float s );
	v4		operator*( const v4& o ) const;
	v4		operator/( const v4& o ) const;
	v4		operator+( const v4& o ) const;
	v4		operator-( const v4& o ) const;
	v4		operator-() const;
	v4		operator*( float s ) const;
	v4		operator/( float s ) const;

	// swizzles
	v3 xyz() const;
	v3 xzy() const;
	v3 yxz() const;
	v3 yzx() const;
	v3 zxy() const;
	v3 zyx() const;

	v2 xy() const;
	v2 yx() const;
	v2 yz() const;
	v2 zy() const;
	v2 xz() const;
	v2 zx() const;

	bool	operator==( const v4& o ) const;
	bool	operator!=( const v4& o ) const;
};

bool check( const v4& v );

float	length( const v4& v );
float	lengthSqr( const v4& v );
float	dot( const v4& a, const v4& b );
float	distance( const v4& p0, const v4& p1 );

v4	operator*( float s, const v4& v );

v4	cross( const v4& a, const v4& b);
v4	normalize( const v4& v );
v4	max( const v4& a, const v4& b );
v4	min( const v4& a, const v4& b );
v4	abs( const v4& v );
v4	lerp( const v4& x, const v4& y, float a );
v4	clamp( const v4& v, const v4& min, const v4& max );
v4	saturate( const v4& v );

#include "v4.inl"
}
