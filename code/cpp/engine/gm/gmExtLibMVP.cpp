#include "gmExtLibMVP.h"

#include "gmThread.h"
#include "gmMachine.h"
#include "gmHelpers.h"

#include <gm/gmBind.h>

#include <math/MVP.h>

namespace funk
{
struct gmfMVPLib
{
	// for the first param "a_mat_stack"
	//  0=Model
	//	1=View
	//	2=Projection
		
	GM_MEMFUNC_DECL(Push)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM( a_mat_stack, 0);
		ASSERT( a_mat_stack < 3 );
		
		MVP::Ref().GetStack(a_mat_stack).Push();
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Pop)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM( a_mat_stack, 0);
		ASSERT( a_mat_stack < 3 );
		
		MVP::Ref().GetStack(a_mat_stack).Pop();
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Translate)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_INT_PARAM( a_mat_stack, 0);
		ASSERT( a_mat_stack < 3 );
		
		if ( GM_IS_PARAM_V2(1) )
		{
			GM_CHECK_V2_PARAM( a_val, 1 );
			MVP::Ref().GetStack(a_mat_stack).Translate(a_val);
		}
		else
		{
			GM_CHECK_V3_PARAM( a_val, 1 );
			MVP::Ref().GetStack(a_mat_stack).Translate(a_val);
		}
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Scale)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_INT_PARAM( a_mat_stack, 0);
		ASSERT( a_mat_stack < 3 );
		
		if ( GM_IS_PARAM_FLOAT(1) )
		{
			GM_CHECK_FLOAT_OR_INT_PARAM( a_val, 1 );
			MVP::Ref().GetStack(a_mat_stack).Scale(v3(a_val));
		}
		else if ( GM_IS_PARAM_V2(1) )
		{
			GM_CHECK_V2_PARAM( a_val, 1 );
			MVP::Ref().GetStack(a_mat_stack).Scale(a_val);
		}
		else
		{
			GM_CHECK_V3_PARAM( a_val, 1 );
			MVP::Ref().GetStack(a_mat_stack).Scale(a_val);
		}
		
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Rotate)
	{
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_INT_PARAM( a_mat_stack, 0);
		GM_CHECK_FLOAT_PARAM( a_radians, 1 );
		GM_CHECK_V3_PARAM( a_axis, 2 );

		ASSERT( a_mat_stack < 3 );
		
		MVP::Ref().GetStack(a_mat_stack).Rotate(a_radians, a_axis);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Reflect)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_INT_PARAM( a_mat_stack, 0);
		GM_CHECK_V3_PARAM( a_plane_normal, 1 );

		ASSERT( a_mat_stack < 3 );
		
		MVP::Ref().GetStack(a_mat_stack).Reflect(a_plane_normal);
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Basis)
	{
		GM_CHECK_NUM_PARAMS(4);
		GM_CHECK_INT_PARAM( a_mat_stack, 0);
		GM_CHECK_V3_PARAM( a_axis_x, 1);
		GM_CHECK_V3_PARAM( a_axis_y, 2 );
		GM_CHECK_V3_PARAM( a_axis_z, 3 );
		
		ASSERT( a_mat_stack < 3 );
		
		MVP::Ref().GetStack(a_mat_stack).Mul( matrix::Basis(a_axis_x, a_axis_y, a_axis_z) );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(LookAt)
	{
		GM_CHECK_NUM_PARAMS(4);
		GM_CHECK_INT_PARAM( a_mat_stack, 0);
		GM_CHECK_V3_PARAM( a_eye_pos, 1);
		GM_CHECK_V3_PARAM( a_target, 2 );
		GM_CHECK_V3_PARAM( a_up, 3 );
		
		ASSERT( a_mat_stack < 3 );
		
		MVP::Ref().GetStack(a_mat_stack).Mul( matrix::LookAt(a_eye_pos, a_target, a_up) );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(LoadIdentity)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM( a_mat_stack, 0);
		ASSERT( a_mat_stack < 3 );
		
		MVP::Ref().GetStack(a_mat_stack).LoadIdentity();
		return GM_OK;
	}
};

static gmFunctionEntry s_gmMVPLib[] =
{
	GM_LIBFUNC_ENTRY(Push, MVP)
	GM_LIBFUNC_ENTRY(Pop, MVP)
	GM_LIBFUNC_ENTRY(Translate, MVP)
	GM_LIBFUNC_ENTRY(Scale, MVP)
	GM_LIBFUNC_ENTRY(Rotate, MVP)
	GM_LIBFUNC_ENTRY(Reflect, MVP)
	GM_LIBFUNC_ENTRY(Basis, MVP)
	GM_LIBFUNC_ENTRY(LoadIdentity, MVP)
	GM_LIBFUNC_ENTRY(LookAt, MVP)
};

void gmBindMVPLib( gmMachine * a_machine )
{
	a_machine->RegisterLibrary(s_gmMVPLib, sizeof(s_gmMVPLib) / sizeof(s_gmMVPLib[0]), "MVP" );
}

}