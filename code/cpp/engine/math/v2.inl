#define V2_CHECK(v) ASSERT(v)


inline v2::v2() : x(0), y(0) {;}
inline v2::v2(float v) : x(v), y(v) {;}
inline v2::v2(float _x, float _y ) : x(_x), y(_y) {;}

inline bool checkV2( const v2& v )
{
	return v.x<=FLT_MAX && v.x>=-FLT_MAX
	&& v.y<=FLT_MAX && v.y>=-FLT_MAX;
}

inline v2& v2::operator+=( const v2& o )
{
	x += o.x;
	y += o.y;
	V2_CHECK( checkV2(*this) );
	return *this;
}

inline v2& v2::operator-=( const v2& o )
{
	x -= o.x;
	y -= o.y;
	V2_CHECK( checkV2(*this) );
	return *this;
}

inline v2& v2::operator*=( const v2& o )
{
	x *= o.x;
	y *= o.y;
	V2_CHECK( checkV2(*this) );
	return *this;
}

inline v2& v2::operator/=( const v2& o )
{
	V2_CHECK( fabsf(o.x) > FLT_MIN );
	V2_CHECK( fabsf(o.y) > FLT_MIN );

	x /= o.x;
	y /= o.y;
	V2_CHECK( checkV2(*this) );
	return *this;
}

inline v2& v2::operator*=( float s )
{
	x *= s;
	y *= s;
	V2_CHECK( checkV2(*this) );
	return *this;
}

inline v2& v2::operator/=( float s )
{
	float div = 1.0f/s;
	x *= div;
	y *= div;
	V2_CHECK( checkV2(*this) );
	return *this;
}

inline v2 v2::operator*( const v2& o ) const
{
	V2_CHECK( checkV2(o) );
	V2_CHECK( checkV2(*this) );
	return v2(x*o.x, y*o.y);
}

inline v2 v2::operator/( const v2& o ) const
{
	V2_CHECK( checkV2(o) );
	V2_CHECK( checkV2(*this) );
	V2_CHECK( fabsf(o.x) > FLT_MIN );
	V2_CHECK( fabsf(o.y) > FLT_MIN );
	return v2(x/o.x, y/o.y);
}

inline v2 v2::operator+( const v2& o ) const
{
	V2_CHECK( checkV2(o) );
	V2_CHECK( checkV2(*this) );
	return v2(x+o.x, y+o.y);
}

inline v2 v2::operator-( const v2& o ) const
{
	V2_CHECK( checkV2(o) );
	V2_CHECK( checkV2(*this) );
	return v2(x-o.x, y-o.y);
}

inline v2 v2::operator-() const
{
	V2_CHECK( checkV2(*this) );
	return v2(-x, -y);
}

inline v2 v2::operator*( float s ) const
{
	V2_CHECK( checkV2(*this) );
	return v2(x*s, y*s);
}

inline v2 v2::operator/( float s ) const
{
	V2_CHECK( fabsf(s) > FLT_MIN );
	V2_CHECK( checkV2(*this) );
	float div = 1.0f / s;
	return v2(x*div, y*div);
}

inline bool	v2::operator==( const v2& o ) const
{
	return x == o.x && y == o.y;
}
inline bool v2::operator!=( const v2& o ) const
{
	return !(*this == o);
}

inline float length( const v2& v )
{
	return sqrtf( lengthSqr(v) );
}

inline float lengthSqr( const v2& v )
{
	return v.x*v.x + v.y*v.y;
}

inline float dot( const v2& a, const v2& b )
{
	return a.x*b.x + a.y*b.y;
}

inline float distance( const v2& p0, const v2& p1 )
{
	return length( p1 - p0 );
}

inline v2 operator*( float s, const v2& v )
{
	return v2( v.x*s, v.y*s );
}

inline v2 normalize( const v2& v )
{
	float len = length(v);
	V2_CHECK( fabs(len) > FLT_MIN );
	float divLen = 1.0f / len;
	return v * divLen;
}

inline v2 max( const v2& a, const v2& b )
{
	return v2( max(a.x, b.x), max(a.y, b.y) );
}

inline v2 min( const v2& a, const v2& b )
{
	return v2( min(a.x, b.x), min(a.y, b.y) );
}

inline v2 abs( const v2& v )
{
	return v2( fabs(v.x), fabs(v.y) );
}

inline v2 lerp( const v2& x, const v2& y, float a )
{
	return v2( lerp(x.x, y.x, a), lerp( x.y, y.y, a) );
}

inline v2 clamp( const v2& v, const v2& min, const v2& max )
{
	return v2( clamp(v.x, min.x, max.x), clamp(v.y, min.y, max.y) );
}

inline v2 saturate( const v2& v )
{
	return v2( clamp(v.x, 0.0f, 1.0f), clamp(v.y, 0.0f, 1.0f ) );
}

inline v2 perpendicular( const v2& v )
{
	return v2(-v.y,v.x);
}

inline v2 unitCircle( float angleRad )
{
	return v2( cosf(angleRad), sinf(angleRad) );
}

inline v2 hermite( const v2& v0, const v2& tan0, const v2& v1, const v2& tan1, float t )
{
	float t_cubed = t*t*t;
	float t_sqr = t*t;

	float h1 =  2.0f*t_cubed - 3.0f*t_sqr + 1.0f;   // calculate basis function 1
	float h2 = -2.0f*t_cubed + 3.0f*t_sqr;          // calculate basis function 2
	float h3 =  t_cubed - 2.0f*t_sqr + t;			// calculate basis function 3
	float h4 =  t_cubed -  t_sqr;					// calculate basis function 4

	return h1*v0 + h2*v1 + h3*tan0 + h4*tan1;
}