
#pragma once

#include <assert.h>
#include <math.h>
#include <float.h>
#include <math/Util.h>

namespace funk
{
class Complex
{
public:
	union
	{
		struct { float re, im; };
	};

	Complex();
	Complex( float v );
	Complex( float _re, float _im );
	Complex( const Complex &c )  : re(c.re), im(c.im) {;}

	float&			operator[]( size_t i )				{assert( i < 2 ); return (&re)[i];}
	const float&	operator[]( size_t i ) const		{assert( i < 2 ); return (&re)[i];}

	Complex&		operator+=( const Complex& o );
	Complex&		operator-=( const Complex& o );
	Complex&		operator*=( const Complex& o );
	Complex&		operator*=( float s );
	Complex&		operator/=( float s );
	
	Complex		operator*( const Complex& o ) const;
	Complex		operator/( const Complex& o ) const;
	Complex		operator+( const Complex& o ) const;
	Complex		operator-( const Complex& o ) const;
	Complex		operator-() const;
	Complex		operator*( float s ) const;
	Complex		operator/( float s ) const;

	bool	operator==( const Complex& o ) const;
	bool	operator!=( const Complex& o ) const;
};

bool check( const Complex& v );

float	length( const Complex& v );
float	lengthSqr( const Complex& v );;
float	distance( const Complex& p0, const Complex& p1 );

Complex euler( float radians );
Complex normalize( const Complex & c );
Complex conjugate( const Complex & c );
Complex	operator*( float s, const Complex& v );

#include "Complex.inl"
}
