#define V3_CHECK(v) ASSERT(v)

inline v3::v3() : x(0), y(0), z(0) {;}
inline v3::v3(float v) : x(v), y(v), z(v) {;}
inline v3::v3(float _x, float _y, float _z ) : x(_x), y(_y), z(_z) {;}

inline v3& v3::operator+=( const v3& o )
{
	x += o.x;
	y += o.y;
	z += o.z;
	V3_CHECK( checkV3(*this) );
	return *this;
}

inline v3& v3::operator-=( const v3& o )
{
	x -= o.x;
	y -= o.y;
	z -= o.z;
	V3_CHECK( checkV3(*this) );
	return *this;
}

inline v3& v3::operator*=( const v3& o )
{
	x *= o.x;
	y *= o.y;
	z *= o.z;
	V3_CHECK( checkV3(*this) );
	return *this;
}

inline v3& v3::operator/=( const v3& o )
{
	V3_CHECK( fabsf(o.x) > FLT_MIN );
	V3_CHECK( fabsf(o.y) > FLT_MIN );
	V3_CHECK( fabsf(o.z) > FLT_MIN );

	x /= o.x;
	y /= o.y;
	z /= o.z;
	
	V3_CHECK( checkV3(*this) );
	return *this;
}

inline v3& v3::operator*=( float s )
{
	x *= s;
	y *= s;
	z *= s;
	V3_CHECK( checkV3(*this) );
	return *this;
}

inline v3& v3::operator/=( float s )
{
	float div = 1.0f/s;
	x *= div;
	y *= div;
	z *= div;
	V3_CHECK( checkV3(*this) );
	return *this;
}

inline v3 v3::operator*( const v3& o ) const
{
	V3_CHECK( checkV3(o) );
	V3_CHECK( checkV3(*this) );
	return v3(x*o.x, y*o.y, z*o.z);
}

inline v3 v3::operator/( const v3& o ) const
{
	V3_CHECK( checkV3(o) );
	V3_CHECK( checkV3(*this) );
	V3_CHECK( fabsf(o.x) > FLT_MIN );
	V3_CHECK( fabsf(o.y) > FLT_MIN );
	V3_CHECK( fabsf(o.z) > FLT_MIN );
	return v3(x/o.x, y/o.y, z/o.z);
}

inline v3 v3::operator+( const v3& o ) const
{
	V3_CHECK( checkV3(o) );
	V3_CHECK( checkV3(*this) );
	return v3(x+o.x, y+o.y, z+o.z);
}

inline v3 v3::operator-( const v3& o ) const
{
	V3_CHECK( checkV3(o) );
	V3_CHECK( checkV3(*this) );
	return v3(x-o.x, y-o.y, z-o.z);
}

inline v3 v3::operator-() const
{
	V3_CHECK( checkV3(*this) );
	return v3(-x, -y, -z);
}

inline v3 v3::operator*( float s ) const
{
	V3_CHECK( checkV3(*this) );
	return v3(x*s, y*s, z*s);
}

inline v3 v3::operator/( float s ) const
{
	V3_CHECK( fabsf(s) > FLT_MIN );
	V3_CHECK( checkV3(*this) );
	float div = 1.0f / s;
	return v3(x*div, y*div, z*div);
}

inline bool	v3::operator==( const v3& o ) const
{
	return x == o.x && y == o.y && z == o.z;
}
inline bool v3::operator!=( const v3& o ) const
{
	return !(*this == o);
}

inline bool checkV3( const v3& v )
{
	return v.x<=FLT_MAX && v.x>=-FLT_MAX 
		&& v.y<=FLT_MAX && v.y>=-FLT_MAX && v.z<=FLT_MAX && v.z>=-FLT_MAX;
}

inline float length( const v3& v )
{
	return sqrtf( lengthSqr(v) );
}

inline float lengthSqr( const v3& v )
{
	return v.x*v.x + v.y*v.y + v.z*v.z;
}

inline float dot( const v3& a, const v3& b )
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline float distance( const v3& p0, const v3& p1 )
{
	return length( p1 - p0 );
}

inline v3 operator*( float s, const v3& v )
{
	return v3( v.x*s, v.y*s, v.z*s );
}

inline v3 cross( const v3& a, const v3& b)
{
	v3 res( a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x );
	V3_CHECK( checkV3(res) );
	return res;
}

inline v3 normalize( const v3& v )
{
	float len = length(v);
	V3_CHECK( fabs(len) > FLT_MIN );
	float divLen = 1.0f / len;
	return v * divLen;
}

inline v3 max( const v3& a, const v3& b )
{
	return v3( max(a.x, b.x), max(a.y, b.y), max(a.z, b.z) );
}

inline v3 min( const v3& a, const v3& b )
{
	return v3( min(a.x, b.x), min(a.y, b.y), min(a.z, b.z) );
}

inline v3 abs( const v3& v )
{
	return v3( fabs(v.x), fabs(v.y), fabs(v.z) );
}

inline v3 lerp( const v3& x, const v3& y, float a )
{
	return v3( lerp(x.x, y.x, a), lerp( x.y, y.y, a), lerp( x.z, y.z, a) );
}

inline v3 clamp( const v3& v, const v3& min, const v3& max )
{
	return v3( clamp(v.x, min.x, max.x), clamp(v.y, min.y, max.y), clamp(v.z, min.z, max.z) );
}

inline v3 saturate( const v3& v )
{
	return v3( clamp(v.x, 0.0f, 1.0f), clamp(v.y, 0.0f, 1.0f ), clamp(v.z, 0.0f, 1.0f ) );
}

inline v3 perpendicular( const v3& v )
{
	v3 perp = cross(v, v3(1.0f,0.0f,0.0f) );

	static const float epsilon = 0.0000001f;
	if ( length(perp) < epsilon )
	{
		perp = cross(v, v3(0.0f,1.0f,0.0f) );
	}

	return perp;
}

inline v3 hermite( const v3& v0, const v3& tan0, const v3& v1, const v3& tan1, float t )
{
	float t_cubed = t*t*t;
	float t_sqr = t*t;

	float h1 =  2.0f*t_cubed - 3.0f*t_sqr + 1.0f;   // calculate basis function 1
	float h2 = -2.0f*t_cubed + 3.0f*t_sqr;          // calculate basis function 2
	float h3 =  t_cubed - 2.0f*t_sqr + t;			// calculate basis function 3
	float h4 =  t_cubed -  t_sqr;					// calculate basis function 4

	return h1*v0 + h2*v1 + h3*tan0 + h4*tan1;
}

inline v3 facenormal_cw( const v3& v0, const v3& v1, const v3& v2 )
{
	v3 dir0 = v1 - v0;
	v3 dir1 = v2 - v0;

	return normalize( cross( dir0, dir1 ) );
}

inline v3 facenormal_ccw( const v3& v0, const v3& v1, const v3& v2 )
{
	v3 dir0 = v1 - v0;
	v3 dir1 = v2 - v0;

	return normalize( cross( dir1, dir0 ) );
}

inline v3 v3::randSpherePt()
{
	return normalize(v3(randbetween(-1.0f,1.0f),randbetween(-1.0f,1.0f),randbetween(-1.0f,1.0f)));
}

inline v3 rotateAxisX( const v3& dir, float angle )
{
	float cosVal = cosf(angle);
	float sinVal = sinf(angle);

	float newLookY = dir.y*cosVal - dir.z*sinVal;
	float newLookZ = dir.y*sinVal + dir.z*cosVal;
	float newLookX = dir.x;

	return v3(newLookX,newLookY,newLookZ);
}

inline v3 rotateAxisY( const v3& dir, float angle )
{
	float cosVal = cosf(angle);
	float sinVal = sinf(angle);

	float newLookZ = dir.z*cosVal - dir.x*sinVal;
	float newLookX = dir.z*sinVal + dir.x*cosVal;
	float newLookY = dir.y;

	return v3(newLookX,newLookY,newLookZ);
}

inline v3 rotateAxisZ( const v3& dir, float angle )
{
	float cosVal = cosf(angle);
	float sinVal = sinf(angle);

	float newLookX = dir.x*cosVal - dir.y*sinVal;
	float newLookY = dir.x*sinVal + dir.y*cosVal;
	float newLookZ = dir.z;

	return v3(newLookX,newLookY,newLookZ);
}

inline v3 rotateAxis( const v3& dir, float angle, v3 axis )
{
	// http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/
	float cosVal = cosf(angle);
	float cosValMinus = 1.0f-cosVal;
	float sinVal = sinf(angle);

	float x = dir.x;
	float y = dir.y;
	float z = dir.z;

	float u = axis.x;
	float v = axis.y;
	float w = axis.z;

	float axisLenSqr = lengthSqr(axis);
	float axisLen = sqrtf(axisLenSqr);
	float c0 = u*x + v*y + w*z;

	float xVal = u*c0*cosValMinus + axisLenSqr*x*cosVal + axisLen*(-w*y+v*z)*sinVal;
	float yVal = v*c0*cosValMinus + axisLenSqr*y*cosVal + axisLen*(w*x-u*z)*sinVal;
	float zVal = w*c0*cosValMinus + axisLenSqr*z*cosVal + axisLen*(-v*x+u*y)*sinVal;

	v3 result = v3(xVal,yVal,zVal) / axisLenSqr;
	return result;
}