inline matrix::matrix() : x(1.0f,0.0f,0.0f,0.0f), y(0.0f,1.0f,0.0f,0.0f), z(0.0f,0.0f,1.0f,0.0f), w(0.0f,0.0f,0.0f,1.0f) {;}
inline matrix::matrix( const v4 _x, const v4 _y, const v4 _z, const v4 _w ) : x(_x), y(_y), z(_z), w(_w) {;}
inline matrix::matrix( const matrix& other ) : x(other.x), y(other.y), z(other.z), w(other.w){;}

inline v3 matrix::getTranslation() const
{
	return v3(w.x, w.y, w.z);
}

inline matrix matrix::transpose() const
{
	matrix result;
	result.x = v4(x.x, y.x, z.x, w.x);
	result.y = v4(x.y, y.y, z.y, w.y);
	result.z = v4(x.z, y.z, z.z, w.z);
	result.w = v4(x.w, y.w, z.w, w.w);

	return result;
}

inline matrix matrix::Identity()
{
	return matrix(
		v4(1.0f,0.0f,0.0f,0.0f),
		v4(0.0f,1.0f,0.0f,0.0f),
		v4(0.0f,0.0f,1.0f,0.0f),
		v4(0.0f,0.0f,0.0f,1.0f));
}

inline matrix matrix::Zero()
{
	return matrix(v4(0.0f),v4(0.0f),v4(0.0f),v4(0.0f));
}

inline matrix matrix::Translation( const v2 value )
{
	matrix result = matrix::Identity();
	
	result.w.x = value.x;
	result.w.y = value.y;

	return result;
}

inline matrix matrix::Translation( const v3 value )
{
	matrix result = matrix::Identity();
	
	result.w.x = value.x;
	result.w.y = value.y;
	result.w.z = value.z;

	return result;
}

inline matrix matrix::Translation( const v4 value )
{
	return matrix::Translation( v3( value.x, value.y, value.z ) );
}

inline matrix matrix::Scale( const v3 value )
{
	return matrix(
		v4(value.x,0.0f,0.0f,0.0f), 
		v4(0.0f,value.y,0.0f,0.0f),
		v4(0.0f,0.0f,value.z,0.0f),
		v4(0.0f,0.0f,0.0f,1.0f));
}

inline matrix matrix::Basis( const v3 x, const v3 y, const v3 z )
{
	return matrix(
		v4(x.x, x.y, x.z, 0.0f), 
		v4(y.x, y.y, y.z, 0.0f), 
		v4(z.x, z.y, z.z, 0.0f), 
		v4(0.0f,0.0f,0.0f,1.0f));
}

inline matrix matrix::Rotation( const float angle, const v4 axis )
{
	return matrix::Rotation( angle, v3(axis.x, axis.y, axis.z) );
}

inline matrix matrix::Rotation( const float angle, const v3 axis )
{
	const float c = cosf( angle );
	const v3 s ( axis * sinf( angle ) );
	const v3 t ( axis * ( 1.0f - c ) );

	return matrix( v4 ( t.x * axis.x + c   , t.y * axis.x + s.z , t.z * axis.x - s.y , 0.f ),
				   v4 ( t.x * axis.y - s.z , t.y * axis.y + c   , t.z * axis.y + s.x , 0.f ),
				   v4 ( t.x * axis.z + s.y , t.y * axis.z - s.x , t.z * axis.z + c     , 0.f ),   
				   v4(0.0f,0.0f,0.0f,1.0f) );
}

inline matrix matrix::Reflect( const v4 a_plane_normal )
{
	float a = a_plane_normal.x;
	float b = a_plane_normal.y;
	float c = a_plane_normal.z;

	return matrix( v4 ( 1.0f-2.0f*a*a, -2.0f*a*b, -2.0f*a*c, 0.f ),
				   v4 ( -2.0f*a*b, 1.0f-2.0f*b*b, -2.0f*b*c, 0.f ),
				   v4 ( -2.0f*a*c, -2.0f*b*c, 1.0f-2.0f*c*c, 0.f ),   
				   v4(0.0f,0.0f,0.0f,1.0f) );
}

inline void matrix::cancelTranslation()
{
	w.x = 0.0f;
	w.y = 0.0f;
	w.z = 0.0f;
}


inline matrix matrix::Frustum( const float l,  const float r,  
							  const float b,  const float t, 
							  const float n,  const float f )
{
	const float E = 2.f * n / ( r - l );
	const float F = 2.f * n / ( t - b );

	const float A = ( r + l ) / ( r - l );
	const float B = ( t + b ) / ( t - b );
	const float C = - ( f + n ) / ( f - n );
	const float D = - 2.f * f * n / ( f - n );

	return matrix( v4( E   , 0.f , 0.f , 0.f ),
				   v4( 0.f , F   , 0.f , 0.f ),
				   v4( A   , B   , C   ,-1.f ),
				   v4( 0.f , 0.f , D   , 0.f ) );
}

inline matrix matrix::Ortho( const float l, const float r,  
							const float b, const float t, 
							const float n, const float f )
{
	const float r_minus_l_inv = 1.f / ( r - l );
	const float t_minus_b_inv = 1.f / ( t - b );
	const float n_minus_f_inv = 1.f / ( n - f );

	return matrix( v4( 2.f * r_minus_l_inv, 0.f, 0.f, 0.f ),
				   v4( 0.f, 2.f * t_minus_b_inv, 0.f, 0.f ),
				   v4( 0.f, 0.f, 2.f * n_minus_f_inv, 0.f ),
				   v4(
					 - ( r + l ) * r_minus_l_inv,
					 - ( t + b ) * t_minus_b_inv,
					 ( f + n ) * n_minus_f_inv, 1.f ) );
}

inline matrix matrix::Perspective( const float fovy, const float aspect, const float n, const float f )
{
	const float t = n * tanf( fovy * 0.5f );
	const float b = -t;
	const float r = t * aspect;
	const float l = -r;

	return matrix::Frustum(l,r,b,t,n,f);
}

inline float matrix::determinant3x3() const
{
	return x.x * y.y * z.z +
			y.x * z.y * x.z +
			z.x * x.y * y.z -
			z.x * y.y * x.z -
			y.x * x.y * z.z -
			x.x * z.y * y.z ;
}

inline v4 matrix::operator * ( const v4 value ) const
{
	matrix tranposed = transpose();

	return v4( 
		dot(tranposed.x,value), 
		dot(tranposed.y,value),
		dot(tranposed.z,value),
		dot(tranposed.w,value) );
}

inline v3 matrix::operator * ( const v3 value ) const
{
	v4 result = (*this) * v4(value.x, value.y, value.z, 0.0f);
	return v3(result.x, result.y, result.z);
}

inline matrix matrix::operator * ( const matrix& mat ) const
{
	matrix t = transpose();

	v4 vecX = v4( dot(t.x,mat.x), dot(t.y,mat.x), dot(t.z,mat.x),  dot(t.w,mat.x) );
	v4 vecY = v4( dot(t.x,mat.y), dot(t.y,mat.y), dot(t.z,mat.y),  dot(t.w,mat.y) );
	v4 vecZ = v4( dot(t.x,mat.z), dot(t.y,mat.z), dot(t.z,mat.z),  dot(t.w,mat.z) );
	v4 vecW = v4( dot(t.x,mat.w), dot(t.y,mat.w), dot(t.z,mat.w),  dot(t.w,mat.w) );

	return matrix(vecX, vecY, vecZ, vecW);
}

inline void matrix::operator *= ( const matrix& mat )
{
	*this = *this * mat;
}

inline void matrix::operator *= ( const float val )
{
	x *= val;
	y *= val;
	z *= val;
	w *= val;
}

inline matrix matrix::operator * ( const float val ) const
{
	matrix result = *this;
	result *= val;
	return result;
}


inline matrix matrix::operator + ( const matrix& mat ) const
{
	return matrix( x+mat.x, y+mat.y, z+mat.z, w+mat.w );
}

inline matrix matrix::operator - ( const matrix& mat ) const
{
	return matrix( x-mat.x, y-mat.y, z-mat.z, w-mat.w );
}

inline void matrix::operator += ( const matrix& mat )
{
	x += mat.x;
	y += mat.y;
	z += mat.z;
	w += mat.w;
}

inline void matrix::operator -= ( const matrix& mat )
{
	x -= mat.x;
	y -= mat.y;
	z -= mat.z;
	w -= mat.w;
}

inline bool matrix::operator == ( const matrix& mat ) const
{
	return x == mat.x && y == mat.y && z == mat.z && w == mat.w;
}

inline bool matrix::operator != ( const matrix& mat ) const
{
	return !(*this == mat );
}

inline float matrix::extractFovy() const
{
	return 2.f*atanf(1.f/y.y);	
}

inline float matrix::extractAspect() const
{
	return y.y/x.x;
}

inline float matrix::extractFocalLength() const
{
	return x.x;
}


inline float matrix::extractZnear() const
{
	return w.z/(z.z-1.f);
}

inline float matrix::extractZfar() const
{
	return w.z/(z.z+1.f);
}