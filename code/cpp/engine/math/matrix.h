
#pragma once

#include <assert.h>
#include <math.h>

#include "v2.h"
#include "v3.h"
#include "v4.h"

namespace funk
{

class matrix
{
public:

	/*
	OpenGL specifies matrices as a 
	one-dimensional array listed in 
	column-major order, ie with elements 
	ordered like this:

		x  y  z    w

		m0 m4 m8  m12
		m1 m5 m9  m13
		m2 m6 m10 m14
		m3 m7 m11 m15

	x,y,z are axis, w may be translation
	openGL camera looks down +Z axis

	http://www.songho.ca/opengl/gl_transform.html#modelview
	http://www.songho.ca/opengl/gl_matrix.html
	*/

	v4 x, y, z, w; // columns

public:
	matrix(); // identity
	matrix( const v4 _x, const v4 _y, const v4 _z, const v4 _w );
	matrix( const matrix& other );
	matrix( float _data[16] ) : x(_data[0],_data[1],_data[2],_data[3]), y(_data[4],_data[5],_data[6],_data[7]) , z(_data[8],_data[9],_data[10],_data[11]), w(_data[12],_data[13],_data[14],_data[15])  {;}

	matrix transpose() const;

	// translation
	v3 getTranslation() const;
	void cancelTranslation();

	float determinant3x3() const; // top-left 3x3
	matrix inverse3x3() const;
	matrix inverse4x4() const;
	matrix inverseRT() const; // if only rotation and translation

	static matrix Identity();
	static matrix Zero();
	static matrix Translation( const v2 value );
	static matrix Translation( const v3 value );
	static matrix Translation( const v4 value );
	static matrix Scale( const v3 value );
	static matrix Basis( const v3 x, const v3 y, const v3 z );
	static matrix Rotation( const float angle, const v4 axis );
	static matrix Rotation( const float angle, const v3 axis );
	static matrix Reflect( const v4 a_plane_normal );
	static matrix LookAt( const v3 eyePos, const v3 target, const v3 up );

	// projection
	static matrix Perspective( const float fovy, const float aspect, const float near, const float far );
	static matrix Frustum(  const float l,  const float r,  
							const float b,  const float t, 
							const float n,  const float f );
	static matrix Ortho( const float l,  const float r,  
						 const float b,  const float t, 
						 const float n,  const float f );

	float extractFovy() const;
	float extractAspect() const;
	float extractZnear() const;
	float extractZfar() const;
	float extractFocalLength() const;

	// operators
	v4		operator * ( const v4 value ) const;
	v3		operator * ( const v3 value ) const;
	void	operator *= ( const float );
	matrix	operator * ( const float ) const;
	matrix	operator * ( const matrix& mat ) const;
	void	operator *= ( const matrix& mat );
	matrix	operator + ( const matrix& mat ) const;
	matrix	operator - ( const matrix& mat ) const;
	void	operator += ( const matrix& mat );
	void	operator -= ( const matrix& mat );
	bool	operator == ( const matrix& mat ) const;
	bool	operator != ( const matrix& mat ) const;
	
	float* GetFloatPtr() { return &x[0]; }

	v4&				operator[]( size_t i )				{assert( i < 4 ); return (&x)[i];}
	const v4&		operator[]( size_t i ) const		{assert( i < 4 ); return (&x)[i];}
	//float&			operator[]( size_t i )				{assert( i < 16 ); return (&x[0])[i];}
	//const float&	operator[]( size_t i ) const		{assert( i < 16 ); return (&x[0])[i];}
};

#include "matrix.inl"
}
