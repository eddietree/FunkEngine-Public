#include "CubicSpline2d.h"

#include <math.h>

#ifdef GAMEMONKEY_INCLUDED
#include <gm/gmBind.h>
#endif

namespace funk
{
v2 CubicSpline2d::CalcPosAt( float t ) const
{
	if ( m_controlPts.size() < 2 )
    {
        return v2( -1.0f );
    }
	
	// clamp t-value
	t = saturate(t);

	float dt = 1.0f/(m_controlPts.size()-1);
	int index0 = (int)(t / dt);
	int index1 = index0+1;

	const float lerpFactor = fmod(t,dt) / dt;
	const v2 tan0 = GetTangentAt(index0);
	const v2 tan1 = GetTangentAt(index1);
	const v2 pt0 = GetControlPt(index0);
	const v2 pt1 = GetControlPt(index1);

	float tOne = lerpFactor;
	float tSqr = lerpFactor*lerpFactor;
	float tCube = lerpFactor*lerpFactor*lerpFactor;

	float t0 = 2.0f*tCube - 3.0f*tSqr + 1.0f;
	float t1 = tCube - 2.0f*tSqr + tOne;
	float t2 = -2.0f*tCube + 3.0f*tSqr;
	float t3 = tCube - tSqr;

	v2 result = t0*pt0 + t1*(pt1.x-pt0.x)*tan0 + t2*pt1 + t3*(pt1.x-pt0.x)*tan1;
	return result;
}

void CubicSpline2d::SortControlPtsByX()
{
	for( size_t i = 0; i < m_controlPts.size()-1; ++i )
	{
		for( size_t j = i+1; j < m_controlPts.size(); ++j )
		{
			if ( m_controlPts[j].x < m_controlPts[i].x )
			{
				v2 temp = m_controlPts[i];
				m_controlPts[i] = m_controlPts[j];
				m_controlPts[j] = temp;
			}
		}
	}
}

void CubicSpline2d::SortControlPtsByY()
{
	for( size_t i = 0; i < m_controlPts.size()-1; ++i )
	{
		for( size_t j = i+1; j < m_controlPts.size(); ++j )
		{
			if ( m_controlPts[j].y < m_controlPts[i].y )
			{
				v2 temp = m_controlPts[i];
				m_controlPts[i] = m_controlPts[j];
				m_controlPts[j] = temp;
			}
		}
	}
}

int CubicSpline2d::NumControlPts() const 
{ 
	return (int)m_controlPts.size(); 
}

const v2 CubicSpline2d::GetControlPt(int i) const 
{ 
	assert( i >= 0 && i < (int)m_controlPts.size() );
	return m_controlPts[i]; 
}

void CubicSpline2d::AddControlPt( v2 pt )
{
	m_controlPts.push_back(pt);
}

v2 CubicSpline2d::GetTangentAt( int i ) const
{
	if ( i == 0 ) return ( m_controlPts[i+1]-m_controlPts[i] );
	if ( i == m_controlPts.size()-1 ) return ( m_controlPts[i]-m_controlPts[i-1] );

	float alphaCurr = GetAlphaAt(i);
	float alphaPrev = GetAlphaAt(i-1);
	float alphaNext = GetAlphaAt(i+1);

	v2 ptCurr = GetControlPt(i);
	v2 ptPrev = GetControlPt(i-1);
	v2 ptNext = GetControlPt(i+1);

	v2 tangent = (ptNext-ptCurr)/(2.0f*(alphaNext-alphaCurr) ) + (ptCurr-ptPrev)/(2.0f*(alphaCurr-alphaPrev) );
	//v2 tangent = (ptNext-ptPrev)/(alphaNext-alphaPrev);

	return tangent;
}

float CubicSpline2d::GetAlphaAt( int i ) const
{
	return float(i)/(m_controlPts.size()-1);
}

void CubicSpline2d::SetControlPt( int i, v2 val )
{
	assert( i >= 0 && i < (int)m_controlPts.size() );
	m_controlPts[i] = val;
}

void CubicSpline2d::RemoveControlPt()
{
	if ( m_controlPts.empty() ) return;
	m_controlPts.pop_back();
}

#ifdef GAMEMONKEY_INCLUDED
GM_REG_NAMESPACE(CubicSpline2d)
{
	GM_MEMFUNC_CONSTRUCTOR(CubicSpline2d)
	{
		GM_CHECK_NUM_PARAMS(0);
		GM_PUSH_USER_HANDLED( CubicSpline2d, new CubicSpline2d() );
		return GM_OK;
	}

	GM_GEN_MEMFUNC_V2_FLOAT( CubicSpline2d, CalcPosAt )
	GM_GEN_MEMFUNC_V2_INT( CubicSpline2d, GetControlPt )
	GM_GEN_MEMFUNC_VOID_V2( CubicSpline2d, AddControlPt )
	GM_GEN_MEMFUNC_INT_VOID( CubicSpline2d, NumControlPts )
	GM_GEN_MEMFUNC_VOID_VOID( CubicSpline2d, RemoveControlPt )
	GM_GEN_MEMFUNC_VOID_VOID( CubicSpline2d, SortControlPtsByX )
	GM_GEN_MEMFUNC_VOID_VOID( CubicSpline2d, SortControlPtsByY )
}

GM_REG_MEM_BEGIN(CubicSpline2d)
GM_REG_MEMFUNC( CubicSpline2d, CalcPosAt )
GM_REG_MEMFUNC( CubicSpline2d, GetControlPt )
GM_REG_MEMFUNC( CubicSpline2d, NumControlPts )
GM_REG_MEMFUNC( CubicSpline2d, AddControlPt )
GM_REG_MEMFUNC( CubicSpline2d, RemoveControlPt )
GM_REG_MEMFUNC( CubicSpline2d, SortControlPtsByX )
GM_REG_MEMFUNC( CubicSpline2d, SortControlPtsByY )
GM_REG_HANDLED_DESTRUCTORS(CubicSpline2d)
GM_REG_MEM_END()
GM_BIND_DEFINE(CubicSpline2d)
#endif

}