#include "Cam2d.h"

#include <gm/gmBind.h>
#include <system/Window.h>
#include "MVP.h"

namespace funk
{
void Cam2d::Begin()
{
	// push perspective camera
	MVP& mvp = MVP::Ref();
	mvp.Proj().Push();
	mvp.Proj().Load( matrix::Ortho(m_bottomLeft.x, m_topRight.x, m_bottomLeft.y, m_topRight.y, m_zNearFar.x, m_zNearFar.y) );
	
	CamBase::Begin();
}

void Cam2d::InitScreenSpace()
{
	const v2 windowDimen = Window::Get()->Sizef();
	InitScreenSpaceSize(windowDimen);
}

void Cam2d::End()
{
	CamBase::End();
	MVP::Ref().Proj().Pop();
}

void Cam2d::SetBounds( v2 bottomLeft, v2 topRight )
{
	m_bottomLeft = bottomLeft;
	m_topRight = topRight;
}

void Cam2d::InitScreenSpaceSize( v2 size )
{
	ASSERT( size != v2(0.0f) );
	
	SetNearFar( v2(-150.0f, 150.0f) );
	
	SetBounds( -size*0.5f, size*0.5f );
	SetPos( v3(size*0.5f) );
	SetDir( v3(0.f, 0.f, -1.f) );
}

GM_REG_NAMESPACE(Cam2d)
{
	GM_MEMFUNC_DECL(CreateCam2d)
	{
		GM_CHECK_NUM_PARAMS(0);
		GM_PUSH_USER_HANDLED( Cam2d, new Cam2d );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetBounds)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V2_PARAM( botLeft, 0 );
		GM_CHECK_V2_PARAM( topRight, 1 );

		GM_GET_THIS_PTR(Cam2d, ptr);
		ptr->SetBounds( botLeft, topRight );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetPos)
	{
		GM_CHECK_NUM_PARAMS(1);

		GM_GET_THIS_PTR(Cam2d, ptr);

		if ( GM_IS_PARAM_V3(0) )
		{
			GM_V3_PARAM( pos, 0 );
			ptr->SetPos(pos);
		}
		else if ( GM_IS_PARAM_V2(0) )
		{
			GM_V2_PARAM( pos, 0 );
			ptr->SetPos( v3(pos.x, pos.y, 0.0f) );
		}
		else
		{
			return GM_EXCEPTION;
		}

		return GM_OK;
	}

	GM_GEN_MEMFUNC_VOID_VOID( Cam2d, Begin )
	GM_GEN_MEMFUNC_VOID_VOID( Cam2d, End )
	GM_GEN_MEMFUNC_VOID_VOID( Cam2d, InitScreenSpace )
	GM_GEN_MEMFUNC_VOID_V2( Cam2d, SetNearFar )
	GM_GEN_MEMFUNC_VOID_V3( Cam2d, SetLookAt )
	GM_GEN_MEMFUNC_VOID_V3( Cam2d, SetDir )
	GM_GEN_MEMFUNC_VOID_V3( Cam2d, SetYup )
	GM_GEN_MEMFUNC_V3_VOID( Cam2d, GetPos )
	GM_GEN_MEMFUNC_V3_VOID( Cam2d, GetDir )
	GM_GEN_MEMFUNC_V3_VOID( Cam2d, GetYup )
	GM_GEN_MEMFUNC_VOID_V2( Cam2d, InitScreenSpaceSize )
}

GM_REG_MEM_BEGIN(Cam2d)
GM_REG_MEMFUNC( Cam2d, Begin )
GM_REG_MEMFUNC( Cam2d, End )
GM_REG_MEMFUNC( Cam2d, InitScreenSpace )
GM_REG_MEMFUNC( Cam2d, InitScreenSpaceSize )
GM_REG_MEMFUNC( Cam2d, SetBounds )
GM_REG_MEMFUNC( Cam2d, GetPos )
GM_REG_MEMFUNC( Cam2d, SetPos )
GM_REG_MEMFUNC( Cam2d, SetLookAt )
GM_REG_MEMFUNC( Cam2d, SetDir )
GM_REG_MEMFUNC( Cam2d, SetYup )
GM_REG_MEMFUNC( Cam2d, SetNearFar )
GM_REG_MEMFUNC( Cam2d, GetPos )
GM_REG_MEMFUNC( Cam2d, GetDir )
GM_REG_MEMFUNC( Cam2d, GetYup )
GM_REG_HANDLED_DESTRUCTORS(Cam2d)
GM_REG_MEM_END()
GM_BIND_DEFINE(Cam2d)

	
}