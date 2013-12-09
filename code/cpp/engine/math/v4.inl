#define V4_CHECK(v) ASSERT(v)

inline v4::v4() : x(0), y(0), z(0), w(0) {;}
inline v4::v4(float v) : x(v), y(v), z(v), w(v) {;}
inline v4::v4(float _x, float _y, float _z, float _w ) : x(_x), y(_y), z(_z), w(_w) {;}
inline v4::v4( const v3 vec ) : x(vec.x), y(vec.y), z(vec.z), w(0.0f) {;}
inline v4::v4( const v3 vec, float _w ) : x(vec.x), y(vec.y), z(vec.z), w(_w) {;}

inline v4& v4::operator+=( const v4& o )
{
	x += o.x;
	y += o.y;
	z += o.z;
	w += o.w;
	V4_CHECK( check(*this) );
	return *this;
}

inline v4& v4::operator-=( const v4& o )
{
	x -= o.x;
	y -= o.y;
	z -= o.z;
	w -= o.w;
	V4_CHECK( check(*this) );
	return *this;
}

inline v4& v4::operator*=( const v4& o )
{
	x *= o.x;
	y *= o.y;
	z *= o.z;
	w *= o.w;
	V4_CHECK( check(*this) );
	return *this;
}

inline v4& v4::operator/=( const v4& o )
{
	V4_CHECK( fabsf(o.x) > FLT_MIN );
	V4_CHECK( fabsf(o.y) > FLT_MIN );
	V4_CHECK( fabsf(o.z) > FLT_MIN );
	V4_CHECK( fabsf(o.w) > FLT_MIN );

	x /= o.x;
	y /= o.y;
	z /= o.z;
	w /= o.w;
	
	V4_CHECK( check(*this) );
	return *this;
}

inline v4& v4::operator*=( float s )
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	V4_CHECK( check(*this) );
	return *this;
}

inline v4& v4::operator/=( float s )
{
	float div = 1.0f/s;
	x *= div;
	y *= div;
	z *= div;
	w *= div;
	V4_CHECK( check(*this) );
	return *this;
}

inline v4 v4::operator*( const v4& o ) const
{
	V4_CHECK( check(o) );
	V4_CHECK( check(*this) );
	return v4(x*o.x, y*o.y, z*o.z, w*o.w);
}

inline v4 v4::operator/( const v4& o ) const
{
	V4_CHECK( check(o) );
	V4_CHECK( check(*this) );
	V4_CHECK( fabsf(o.x) > FLT_MIN );
	V4_CHECK( fabsf(o.y) > FLT_MIN );
	V4_CHECK( fabsf(o.z) > FLT_MIN );
	V4_CHECK( fabsf(o.w) > FLT_MIN );
	return v4(x/o.x, y/o.y, z/o.z, w/o.w);
}

inline v4 v4::operator+( const v4& o ) const
{
	V4_CHECK( check(o) );
	V4_CHECK( check(*this) );
	return v4(x+o.x, y+o.y, z+o.z, w+o.w);
}

inline v4 v4::operator-( const v4& o ) const
{
	V4_CHECK( check(o) );
	V4_CHECK( check(*this) );
	return v4(x-o.x, y-o.y, z-o.z, w-o.w);
}

inline v4 v4::operator-() const
{
	V4_CHECK( check(*this) );
	return v4(-x, -y, -z, -w);
}

inline v4 v4::operator*( float s ) const
{
	V4_CHECK( check(*this) );
	return v4(x*s, y*s, z*s, w*s);
}

inline v4 v4::operator/( float s ) const
{
	V4_CHECK( fabsf(s) > FLT_MIN );
	V4_CHECK( check(*this) );
	float div = 1.0f / s;
	return v4(x*div, y*div, z*div, w*div);
}

inline v3 v4::xyz() const { return v3(x,y,z); }
inline v3 v4::xzy() const {	return v3(x,z,y); }
inline v3 v4::yxz() const { return v3(y,x,z); }
inline v3 v4::yzx() const { return v3(y,z,x); }
inline v3 v4::zxy() const {	return v3(z,x,y); }
inline v3 v4::zyx() const {	return v3(z,y,x); }
inline v2 v4::xy() const { return v2(x,y); }
inline v2 v4::yx() const { return v2(y,x); }
inline v2 v4::yz() const { return v2(y,z); }
inline v2 v4::zy() const { return v2(z,y); }
inline v2 v4::xz() const { return v2(x,z); }
inline v2 v4::zx() const { return v2(z,x); }

inline bool	v4::operator==( const v4& o ) const
{
	return x == o.x && y == o.y && z == o.z && w == o.w;
}
inline bool v4::operator!=( const v4& o ) const
{
	return !(*this == o);
}

inline bool check( const v4& v )
{
	return v.x<=FLT_MAX && v.x>=-FLT_MAX 
		&& v.y<=FLT_MAX && v.y>=-FLT_MAX 
		&& v.z<=FLT_MAX && v.z>=-FLT_MAX
		&& v.w<=FLT_MAX && v.w>=-FLT_MAX;
}

inline float length( const v4& v )
{
	return sqrtf( lengthSqr(v) );
}

inline float lengthSqr( const v4& v )
{
	return v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w;
}

inline float dot( const v4& a, const v4& b )
{
	return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

inline float distance( const v4& p0, const v4& p1 )
{
	return length( p1 - p0 );
}

inline v4 operator*( float s, const v4& v )
{
	return v4( v.x*s, v.y*s, v.z*s, v.w*s );
}

inline v4 cross( const v4& a, const v4& b)
{
	v4 res( a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x, 0.0f );
	V4_CHECK( check(res) );
	return res;
}

inline v4 normalize( const v4& v )
{
	float len = length(v);
	V4_CHECK( fabs(len) > FLT_MIN );
	float divLen = 1.0f / len;
	return v * divLen;
}

inline v4 max( const v4& a, const v4& b )
{
	return v4( max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w) );
}

inline v4 min( const v4& a, const v4& b )
{
	return v4( min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w) );
}

inline v4 abs( const v4& v )
{
	return v4( fabs(v.x), fabs(v.y), fabs(v.z), fabs(v.w) );
}

inline v4 lerp( const v4& x, const v4& y, float a )
{
	return v4( lerp(x.x, y.x, a), lerp( x.y, y.y, a), lerp( x.z, y.z, a), lerp( x.w, y.w, a) );
}

inline v4 clamp( const v4& v, const v4& min, const v4& max )
{
	return v4( clamp(v.x, min.x, max.x), clamp(v.y, min.y, max.y), clamp(v.z, min.z, max.z), clamp(v.w, min.w, max.w) );
}

inline v4 saturate( const v4& v )
{
	return v4( clamp(v.x, 0.0f, 1.0f), clamp(v.y, 0.0f, 1.0f ), clamp(v.z, 0.0f, 1.0f ), clamp(v.w, 0.0f, 1.0f ) );
}