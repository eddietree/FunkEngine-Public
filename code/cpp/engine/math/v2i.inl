
inline v2i::v2i() : x(0), y(0) {;}
inline v2i::v2i(int v) : x(v), y(v) {;}
inline v2i::v2i(int _x, int _y ) : x(_x), y(_y) {;}

inline v2i& v2i::operator+=( const v2i& o )
{
	*this = (*this) + o;
	return *this;
}

inline v2i& v2i::operator-=( const v2i& o )
{
	*this = (*this) - o;
	return *this;
}

inline v2i& v2i::operator*=( const v2i& o )
{
	*this = (*this) * o;
	return *this;
}

inline v2i& v2i::operator*=( int s )
{
	x *= s;
	y *= s;
	return *this;
}

inline v2i& v2i::operator/=( int s )
{
	assert( s != 0 );
	x /= s;
	y /= s;
	return *this;
}

inline v2i v2i::operator*( const v2i& o ) const
{
	return v2i( x*o.x , y*o.y );
}

inline v2i v2i::operator/( const v2i& o ) const
{
	assert(o.x != 0 || o.y != 0);
	return v2i( x/o.x, y/o.y );
}

inline v2i v2i::operator+( const v2i& o ) const
{
	return v2i(x+o.x, y+o.y);
}

inline v2i v2i::operator-( const v2i& o ) const
{
	return v2i(x-o.x, y-o.y);
}

inline v2i v2i::operator-() const
{
	return v2i(-x, -y);
}

inline v2i v2i::operator*( int s ) const
{
	return v2i(x*s, y*s);
}

inline v2i v2i::operator/( int s ) const
{
	assert(s!=0);
	return v2i(x/s, y/s);
}

inline bool	v2i::operator==( const v2i& o ) const
{
	return x == o.x && y == o.y;
}

inline bool v2i::operator!=( const v2i& o ) const
{
	return !(*this == o);
}

inline float length( const v2i& v )
{
	return sqrtf( (float)lengthSqr(v) );
}

inline int lengthSqr( const v2i& v )
{
	return v.x*v.x + v.y*v.y;
}

inline int dot( const v2i& a, const v2i& b )
{
	return a.x*b.x + a.y*b.y;
}

inline float distance( const v2i& p0, const v2i& p1 )
{
	return length( p1 - p0 );
}

inline v2i operator*( int s, const v2i& v )
{
	return v2i( v.x*s, v.y*s );
}

inline v2i	max( const v2i& a, const v2i& b )
{
	int x = max( a.x, b.x );
	int y = max( a.y, b.y );

	return v2i(x,y);
}

inline v2i 	min( const v2i & a, const v2i & b )
{
	int x_val = min( a.x, b.x );
	int y_val = min( a.y, b.y );

	return v2i(x_val, y_val);
}

inline v2i 	abs( const v2i & v )
{
	int x_val = ::abs( v.x );
	int y_val = ::abs( v.y );

	return v2i(x_val, y_val);
}

inline v2i 	lerp( const v2i & val0, const v2i & val1, float a )
{
	int x_val = val0.x + int((val1.x-val0.x)*a);
	int y_val = val0.y + int((val1.y-val0.y)*a);
	
	return v2i(x_val, y_val);
}

inline v2i clamp( const v2i & v, const v2i & min, const v2i & max )
{
	int x_val = clamp( v.x, min.x, max.x );
	int y_val = clamp( v.y, min.y, max.y );

	return v2i(x_val, y_val);
}

inline v2i 	saturate( const v2i & v )
{
	return clamp( v, v2i(0), v2i(1) );
}

inline v2i perpendicular( const v2i & v )
{
	return v2i(-v.y, v.x);
}