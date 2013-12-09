
#pragma once

#include <assert.h>
#include <math.h>
#include <float.h>
#include <math/Util.h>
#include <gm/gmVector.h>

namespace funk
{
class v2i
{
public:
	union
	{
		struct { int x, y; };
		gmVec2i gmv2i;
	};

	v2i();
	explicit v2i( int v );
	v2i( int _re, int _im );
	v2i( const v2i &c )  : x(c.x), y(c.y) {;}
	v2i( const gmVec2i &_gmv2i ) { gmv2i = _gmv2i; }

	int&			operator[]( size_t i )			{assert( i < 2 ); return (&x)[i];}
	const int&	operator[]( size_t i ) const		{assert( i < 2 ); return (&x)[i];}

	v2i&		operator+=( const v2i& o );
	v2i&		operator-=( const v2i& o );
	v2i&		operator*=( const v2i& o );
	v2i&		operator*=( int s );
	v2i&		operator/=( int s );
	
	v2i		operator*( const v2i& o ) const;
	v2i		operator/( const v2i& o ) const;
	v2i		operator+( const v2i& o ) const;
	v2i		operator-( const v2i& o ) const;
	v2i		operator-() const;
	v2i		operator*( int s ) const;
	v2i		operator/( int s ) const;

	bool	operator==( const v2i& o ) const;
	bool	operator!=( const v2i& o ) const;
};

float	length( const v2i& v );
int		lengthSqr( const v2i& v );
int		dot( const v2i& a, const v2i& b );
float	distance( const v2i& p0, const v2i& p1 );

v2i	operator*( int s, const v2i& v );

v2i	max( const v2i& a, const v2i& b );
v2i	min( const v2i& a, const v2i& b );
v2i	abs( const v2i& v );
v2i	lerp( const v2i& x, const v2i& y, float a );
v2i	clamp( const v2i& v, const v2i& min, const v2i& max );
v2i	saturate( const v2i& v );
v2i	perpendicular( const v2i& v );

#include "v2i.inl"
}
