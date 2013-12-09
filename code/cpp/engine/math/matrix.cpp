#include "matrix.h"

namespace funk
{
	
matrix matrix::inverse3x3() const
{
	float det = determinant3x3();
	if ( det == 0.f ) return matrix::Identity();

	float invdet = 1.f / det;

	return matrix( v4( (    y.y * z.z - z.y * y.z   ) * invdet,
					   ( -( x.y * z.z - z.y * x.z ) ) * invdet,
					   (    x.y * y.z - y.y * x.z   ) * invdet, 0.f ),
				   v4( ( -( y.x * z.z - z.x * y.z ) ) * invdet,
					   (    x.x * z.z - z.x * x.z   ) * invdet,
					   ( -( x.x * y.z - y.x * x.z ) ) * invdet, 0.f ),
				   v4( (    y.x * z.y - z.x * y.y )   * invdet,
					   ( -( x.x * z.y - z.x * x.y ) ) * invdet,
					   (    x.x * y.y - y.x * x.y   ) * invdet, 0.f ),
				   v4(0.0f,0.0f,0.0f,1.0f) );
}


#if 1
// http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
matrix matrix::inverse4x4() const
{
	matrix mmat = *this;
	matrix inv_mat;
	
	float* inv = inv_mat.GetFloatPtr();
	float* m = mmat.GetFloatPtr();
	
	inv[0] = m[5]  * m[10] * m[15] -
	m[5]  * m[11] * m[14] -
	m[9]  * m[6]  * m[15] +
	m[9]  * m[7]  * m[14] +
	m[13] * m[6]  * m[11] -
	m[13] * m[7]  * m[10];
	
	inv[4] = -m[4]  * m[10] * m[15] +
	m[4]  * m[11] * m[14] +
	m[8]  * m[6]  * m[15] -
	m[8]  * m[7]  * m[14] -
	m[12] * m[6]  * m[11] +
	m[12] * m[7]  * m[10];
	
	inv[8] = m[4]  * m[9] * m[15] -
	m[4]  * m[11] * m[13] -
	m[8]  * m[5] * m[15] +
	m[8]  * m[7] * m[13] +
	m[12] * m[5] * m[11] -
	m[12] * m[7] * m[9];
	
	inv[12] = -m[4]  * m[9] * m[14] +
	m[4]  * m[10] * m[13] +
	m[8]  * m[5] * m[14] -
	m[8]  * m[6] * m[13] -
	m[12] * m[5] * m[10] +
	m[12] * m[6] * m[9];
	
	inv[1] = -m[1]  * m[10] * m[15] +
	m[1]  * m[11] * m[14] +
	m[9]  * m[2] * m[15] -
	m[9]  * m[3] * m[14] -
	m[13] * m[2] * m[11] +
	m[13] * m[3] * m[10];
	
	inv[5] = m[0]  * m[10] * m[15] -
	m[0]  * m[11] * m[14] -
	m[8]  * m[2] * m[15] +
	m[8]  * m[3] * m[14] +
	m[12] * m[2] * m[11] -
	m[12] * m[3] * m[10];
	
	inv[9] = -m[0]  * m[9] * m[15] +
	m[0]  * m[11] * m[13] +
	m[8]  * m[1] * m[15] -
	m[8]  * m[3] * m[13] -
	m[12] * m[1] * m[11] +
	m[12] * m[3] * m[9];
	
	inv[13] = m[0]  * m[9] * m[14] -
	m[0]  * m[10] * m[13] -
	m[8]  * m[1] * m[14] +
	m[8]  * m[2] * m[13] +
	m[12] * m[1] * m[10] -
	m[12] * m[2] * m[9];
	
	inv[2] = m[1]  * m[6] * m[15] -
	m[1]  * m[7] * m[14] -
	m[5]  * m[2] * m[15] +
	m[5]  * m[3] * m[14] +
	m[13] * m[2] * m[7] -
	m[13] * m[3] * m[6];
	
	inv[6] = -m[0]  * m[6] * m[15] +
	m[0]  * m[7] * m[14] +
	m[4]  * m[2] * m[15] -
	m[4]  * m[3] * m[14] -
	m[12] * m[2] * m[7] +
	m[12] * m[3] * m[6];
	
	inv[10] = m[0]  * m[5] * m[15] -
	m[0]  * m[7] * m[13] -
	m[4]  * m[1] * m[15] +
	m[4]  * m[3] * m[13] +
	m[12] * m[1] * m[7] -
	m[12] * m[3] * m[5];
	
	inv[14] = -m[0]  * m[5] * m[14] +
	m[0]  * m[6] * m[13] +
	m[4]  * m[1] * m[14] -
	m[4]  * m[2] * m[13] -
	m[12] * m[1] * m[6] +
	m[12] * m[2] * m[5];
	
	inv[3] = -m[1] * m[6] * m[11] +
	m[1] * m[7] * m[10] +
	m[5] * m[2] * m[11] -
	m[5] * m[3] * m[10] -
	m[9] * m[2] * m[7] +
	m[9] * m[3] * m[6];
	
	inv[7] = m[0] * m[6] * m[11] -
	m[0] * m[7] * m[10] -
	m[4] * m[2] * m[11] +
	m[4] * m[3] * m[10] +
	m[8] * m[2] * m[7] -
	m[8] * m[3] * m[6];
	
	inv[11] = -m[0] * m[5] * m[11] +
	m[0] * m[7] * m[9] +
	m[4] * m[1] * m[11] -
	m[4] * m[3] * m[9] -
	m[8] * m[1] * m[7] +
	m[8] * m[3] * m[5];
	
	inv[15] = m[0] * m[5] * m[10] -
	m[0] * m[6] * m[9] -
	m[4] * m[1] * m[10] +
	m[4] * m[2] * m[9] +
	m[8] * m[1] * m[6] -
	m[8] * m[2] * m[5];
	
	float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
	
	if (det == 0)
		return matrix::Identity();
	
	det = 1.0f / det;
	return inv_mat * det;
	
	//for (i = 0; i < 16; i++) invOut[i] = inv[i] * det;
}
	
#else
matrix matrix::inverse4x4() const
{
	
#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT_GET(_mat_,_r_,_c_) (_mat_[_c_][_r_])
#define MAT_SET(_mat_,_r_,_c_,_val_) (_mat_[_c_][_r_]=_val_)
	
        float wtmp[4][8];
        float m0, m1, m2, m3, s;
        float *r0, *r1, *r2, *r3;
        matrix dst;
	
        r0 = wtmp[0];
        r1 = wtmp[1];
        r2 = wtmp[2];
        r3 = wtmp[3];
	
        r0[0] = MAT_GET((*this),0,0);
        r0[1] = MAT_GET((*this),0,1);
        r0[2] = MAT_GET((*this),0,2);
        r0[3] = MAT_GET((*this),0,3);
        r0[4] = 1.f;
        r0[5] = r0[6] = r0[7] = 0.f;
	
        r1[0] = MAT_GET((*this),1,0);
        r1[1] = MAT_GET((*this),1,1);
        r1[2] = MAT_GET((*this),1,2);
        r1[3] = MAT_GET((*this),1,3);
        r1[5] = 1.f;
        r1[4] = r1[6] = r1[7] = 0.f;
	
        r2[0] = MAT_GET((*this),2,0);
        r2[1] = MAT_GET((*this),2,1);
        r2[2] = MAT_GET((*this),2,2);
        r2[3] = MAT_GET((*this),2,3);
        r2[6] = 1.f;
        r2[4] = r2[5] = r2[7] = 0.f;
		
        r3[0] = MAT_GET((*this),3,0);
        r3[1] = MAT_GET((*this),3,1);
        r3[2] = MAT_GET((*this),3,2);
        r3[3] = MAT_GET((*this),3,3);
        r3[7] = 1.f;
        r3[4] = r3[5] = r3[6] = 0.f;
		
        // choose pivot - or die
        if ( fabsf(r3[0]) > fabsf(r2[0]) ) SWAP_ROWS(r3, r2);
        if ( fabsf(r2[0]) > fabsf(r1[0]) ) SWAP_ROWS(r2, r1);
        if ( fabsf(r1[0]) > fabsf(r0[0]) ) SWAP_ROWS(r1, r0);
        if ( 0.f == r0[0] )
			return Identity();
		
        // eliminate first variable
        m1 = r1[0]/r0[0];
        m2 = r2[0]/r0[0];
        m3 = r3[0]/r0[0];
        s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
        s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
        s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
        s = r0[4];
        if ( s != 0.f )
        {
			r1[4] -= m1 * s;
			r2[4] -= m2 * s;
			r3[4] -= m3 * s;
        }
        s = r0[5];
        if ( s != 0.f )
        {
			r1[5] -= m1 * s;
			r2[5] -= m2 * s;
			r3[5] -= m3 * s;
        }
        s = r0[6];
        if ( s != 0.f )
        {
			r1[6] -= m1 * s;
			r2[6] -= m2 * s;
			r3[6] -= m3 * s;
        }
        s = r0[7];
        if ( s != 0.f )
        {
			r1[7] -= m1 * s;
			r2[7] -= m2 * s;
			r3[7] -= m3 * s;
        }
		
        // choose pivot - or die
        if ( fabsf(r3[1]) > fabsf(r2[1]) ) SWAP_ROWS(r3, r2);
        if ( fabsf(r2[1]) > fabsf(r1[1]) ) SWAP_ROWS(r2, r1);
        if ( 0.f == r1[1] )
			return Identity();
		
        // eliminate second variable
        m2 = r2[1]/r1[1];
        m3 = r3[1]/r1[1];
		
        r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
        r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
		
        s = r1[4];
		
        if ( 0.f != s )
        {
			r2[4] -= m2 * s;
			r3[4] -= m3 * s;
        }
        s = r1[5];
        if ( 0.f != s )
        {
			r2[5] -= m2 * s;
			r3[5] -= m3 * s;
        }
        s = r1[6];
        if ( 0.f != s )
        {
			r2[6] -= m2 * s;
			r3[6] -= m3 * s;
        }
        s = r1[7];
        if ( 0.f != s )
        {
			r2[7] -= m2 * s;
			r3[7] -= m3 * s;
        }
		
        // choose pivot - or die
        if ( fabsf(r3[2])>fabsf(r2[2]) ) SWAP_ROWS(r3, r2);
        if ( 0.f == r2[2] )
			return Identity();
		
        // eliminate third variable
        m3 = r3[2]/r2[2];
		
        r3[3] -= m3 * r2[3];
        r3[4] -= m3 * r2[4];
        r3[5] -= m3 * r2[5];
        r3[6] -= m3 * r2[6];
        r3[7] -= m3 * r2[7];
		
        // last check
        if ( 0.f == r3[3] )
			return Identity();
		
        s = 1.f/r3[3];                           // now back substitute row 3
        r3[4] *= s;
        r3[5] *= s;
        r3[6] *= s;
        r3[7] *= s;
		
        m2 = r2[3];                                     // now back substitute row 2
        s  = 1.f/r2[2];
        r2[4] = s * (r2[4] - r3[4] * m2);
        r2[5] = s * (r2[5] - r3[5] * m2);
        r2[6] = s * (r2[6] - r3[6] * m2);
        r2[7] = s * (r2[7] - r3[7] * m2);
        m1 = r1[3];
        r1[4] -= r3[4] * m1;
        r1[5] -= r3[5] * m1;
        r1[6] -= r3[6] * m1;
        r1[7] -= r3[7] * m1;
        m0 = r0[3];
        r0[4] -= r3[4] * m0;
        r0[5] -= r3[5] * m0;
        r0[6] -= r3[6] * m0;
        r0[7] -= r3[7] * m0;
		
        m1 = r1[2];                                     // now back substitute row 1
        s  = 1.f/r1[1];
        r1[4] = s * (r1[4] - r2[4] * m1);
        r1[5] = s * (r1[5] - r2[5] * m1);
        r1[6] = s * (r1[6] - r2[6] * m1);
        r1[7] = s * (r1[7] - r2[7] * m1);
        m0 = r0[2];
        r0[4] -= r2[4] * m0;
        r0[5] -= r2[5] * m0;
        r0[6] -= r2[6] * m0;
        r0[7] -= r2[7] * m0;
		
        m0 = r0[1];                                     // now back substitute row 0
        s  = 1.f/r0[0];
        r0[4] = s * (r0[4] - r1[4] * m0);
        r0[5] = s * (r0[5] - r1[5] * m0);
        r0[6] = s * (r0[6] - r1[6] * m0);
        r0[7] = s * (r0[7] - r1[7] * m0);
		
        MAT_SET(dst,0,0,r0[4]);
        MAT_SET(dst,0,1,r0[5]),
        MAT_SET(dst,0,2,r0[6]);
        MAT_SET(dst,0,3,r0[7]);
        MAT_SET(dst,1,0,r1[4]);
        MAT_SET(dst,1,1,r1[5]);
        MAT_SET(dst,1,2,r1[6]);
        MAT_SET(dst,1,3,r1[7]);
        MAT_SET(dst,2,0,r2[4]);
        MAT_SET(dst,2,1,r2[5]);
        MAT_SET(dst,2,2,r2[6]);
        MAT_SET(dst,2,3,r2[7]);
        MAT_SET(dst,3,0,r3[4]);
        MAT_SET(dst,3,1,r3[5]);
        MAT_SET(dst,3,2,r3[6]);
        MAT_SET(dst,3,3,r3[7]);
		
        return dst;
		
#undef MAT_SET
#undef MAT_GET
#undef SWAP_ROWS
		
}
#endif

matrix matrix::inverseRT() const
{
	matrix rot = *this;
	rot.cancelTranslation();
	rot = rot.transpose();

	matrix translation_inv = matrix::Translation(-getTranslation());
	return rot * translation_inv;
}

matrix matrix::LookAt( const v3 eyePos, const v3 target, const v3 up )
{
	v3 vec_z	= normalize( eyePos-target );
	v3 vec_x	= normalize( cross( up, vec_z ) );
	v3 vec_y	= normalize( cross( vec_z, vec_x ) );
	
	return matrix( v4(vec_x, 0.0f), v4(vec_y, 0.0f), v4(vec_z, 0.0f), v4(eyePos,1.0f) );
}

}