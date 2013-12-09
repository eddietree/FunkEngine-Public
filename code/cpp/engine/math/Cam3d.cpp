#include "Cam3d.h"

#include <gm/gmBind.h>
#include "MVP.h"

namespace funk
{
Cam3d::Cam3d() : m_fov( 45.0f ), m_aspect( 1.0f )
{;}

void Cam3d::Begin()
{
	MVP& mvp = MVP::Ref();
	mvp.Proj().Push();
	mvp.Proj().Load( matrix::Perspective(m_fov, m_aspect, m_zNearFar.x, m_zNearFar.y) );

	CamBase::Begin();
}

void Cam3d::End()
{
	CamBase::End();
	MVP::Ref().Proj().Pop();
}

void Cam3d::SetViewAngle( float fov, float aspectRatio )
{
	m_fov = fov;
	m_aspect = aspectRatio;
}

GM_REG_NAMESPACE(Cam3d)
{
	GM_MEMFUNC_CONSTRUCTOR(Cam3d)
	{
		GM_CHECK_NUM_PARAMS(0);
		Cam3d * pCam = new Cam3d;
		GM_PUSH_USER_HANDLED( Cam3d, pCam );
		return GM_OK;
	}

	GM_GEN_MEMFUNC_VOID_VOID( Cam3d, Begin )
	GM_GEN_MEMFUNC_VOID_VOID( Cam3d, End )
	GM_GEN_MEMFUNC_VOID_V3( Cam3d, SetPos )
	GM_GEN_MEMFUNC_VOID_V3( Cam3d, SetLookAt )
	GM_GEN_MEMFUNC_VOID_V3( Cam3d, SetDir )
	GM_GEN_MEMFUNC_VOID_V3( Cam3d, SetYup )
	GM_GEN_MEMFUNC_VOID_V2( Cam3d, SetNearFar )
	GM_GEN_MEMFUNC_V3_VOID( Cam3d, GetPos )
	GM_GEN_MEMFUNC_V3_VOID( Cam3d, GetDir )
	GM_GEN_MEMFUNC_V3_VOID( Cam3d, GetYup )
	GM_GEN_MEMFUNC_VOID_FLOAT_FLOAT( Cam3d, SetViewAngle )
}

GM_REG_MEM_BEGIN(Cam3d)
GM_REG_MEMFUNC( Cam3d, Begin )
GM_REG_MEMFUNC( Cam3d, End )
GM_REG_MEMFUNC( Cam3d, SetPos )
GM_REG_MEMFUNC( Cam3d, SetLookAt )
GM_REG_MEMFUNC( Cam3d, SetDir )
GM_REG_MEMFUNC( Cam3d, SetYup )
GM_REG_MEMFUNC( Cam3d, SetNearFar )
GM_REG_MEMFUNC( Cam3d, SetViewAngle )
GM_REG_MEMFUNC( Cam3d, GetPos )
GM_REG_MEMFUNC( Cam3d, GetDir )
GM_REG_MEMFUNC( Cam3d, GetYup )
GM_REG_HANDLED_DESTRUCTORS(Cam3d)
GM_REG_MEM_END()
GM_BIND_DEFINE(Cam3d)

}