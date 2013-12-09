#define COMPLEX_CHECK(v) assert(v)

inline Complex::Complex() : re(0), im(0) {;}
inline Complex::Complex(float v) : re(v), im(0) {;}
inline Complex::Complex(float _re, float _im ) : re(_re), im(_im) {;}

inline Complex& Complex::operator+=( const Complex& o )
{
	*this = (*this) + o;
	COMPLEX_CHECK( check(*this) );
	return *this;
}

inline Complex& Complex::operator-=( const Complex& o )
{
	*this = (*this) - o;
	COMPLEX_CHECK( check(*this) );
	return *this;
}

inline Complex& Complex::operator*=( const Complex& o )
{
	*this = (*this) * o;
	COMPLEX_CHECK( check(*this) );
	return *this;
}

inline Complex& Complex::operator*=( float s )
{
	re *= s;
	im *= s;
	COMPLEX_CHECK( check(*this) );
	return *this;
}

inline Complex& Complex::operator/=( float s )
{
	float div = 1.0f/s;
	re *= div;
	im *= div;
	COMPLEX_CHECK( check(*this) );
	return *this;
}

inline Complex Complex::operator*( const Complex& o ) const
{
	COMPLEX_CHECK( check(o) );
	COMPLEX_CHECK( check(*this) );
	return Complex( re*o.re - im*o.im, re*o.im+o.re*im );
}

inline Complex Complex::operator/( const Complex& o ) const
{
	COMPLEX_CHECK( check(o) );
	COMPLEX_CHECK( check(*this) );
	COMPLEX_CHECK( fabsf(o.re) > FLT_MIN );
	COMPLEX_CHECK( fabsf(o.im) > FLT_MIN );

	const float a = re;
	const float b = im;
	const float c = o.re;
	const float d = o.im;

	return Complex( (a*c+b*d)/(c*c+d*d), (b*c-a*d)/(c*c+d*d) );
}

inline Complex Complex::operator+( const Complex& o ) const
{
	COMPLEX_CHECK( check(o) );
	COMPLEX_CHECK( check(*this) );
	return Complex(re+o.re, im+o.im);
}

inline Complex Complex::operator-( const Complex& o ) const
{
	COMPLEX_CHECK( check(o) );
	COMPLEX_CHECK( check(*this) );
	return Complex(re-o.re, im-o.im);
}

inline Complex Complex::operator-() const
{
	COMPLEX_CHECK( check(*this) );
	return Complex(-re, -im);
}

inline Complex Complex::operator*( float s ) const
{
	COMPLEX_CHECK( check(*this) );
	return Complex(re*s, im*s);
}

inline Complex Complex::operator/( float s ) const
{
	COMPLEX_CHECK( fabsf(s) > FLT_MIN );
	COMPLEX_CHECK( check(*this) );
	float div = 1.0f / s;
	return Complex(re*div, im*div);
}

inline bool	Complex::operator==( const Complex& o ) const
{
	return re == o.re && im == o.im;
}
inline bool Complex::operator!=( const Complex& o ) const
{
	return !(*this == o);
}

inline bool check( const Complex& v )
{
	return v.re<=FLT_MAX && v.re>=-FLT_MAX && v.im<=FLT_MAX && v.im>=-FLT_MAX;
}

inline float length( const Complex& v )
{
	return sqrtf( lengthSqr(v) );
}

inline float lengthSqr( const Complex& v )
{
	return v.re*v.re + v.im*v.im;
}

inline float distance( const Complex& p0, const Complex& p1 )
{
	return length( p1 - p0 );
}

inline Complex euler( float radians )
{
	return Complex( cosf(radians), sinf(radians) );
}

inline Complex conjugate( const Complex & c )
{
	return Complex( c.re, -c.im );
}

inline Complex normalize( const Complex& c )
{
	float len = length(c);
	COMPLEX_CHECK( fabs(len) > FLT_MIN );
	float divLen = 1.0f / len;
	return c * divLen;
}

inline Complex operator*( float s, const Complex& v )
{
	return Complex( v.re*s, v.im*s );
}