#include "gmExtLibInput.h"

#include "gmThread.h"
#include "gmMachine.h"
#include "gmHelpers.h"

#include <gm/gmBind.h>

#include <system/Config.h>
#include <system/Input.h>

namespace funk
{
struct gmfInputLib
{
	// Mouse
	GM_MEMFUNC_DECL(ShowMouseCursor)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM( show, 0 );
		a_thread->PushInt( Input::Get()->ShowMouseCursor(show != 0) );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(IsMouseDown)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM( btn, 0 );
		a_thread->PushInt( Input::Get()->IsMouseDown(btn) ? 1:0 );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(DidMouseJustGoDown)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM( btn, 0 );
		a_thread->PushInt( Input::Get()->DidMouseJustGoDown(btn) ? 1:0 );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(DidMouseJustGoUp)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM( btn, 0 );
		a_thread->PushInt( Input::Get()->DidMouseJustGoUp(btn) ? 1:0 );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(DidMouseDoubleClick)
	{
		GM_CHECK_NUM_PARAMS(0);
		a_thread->PushInt( Input::Get()->DidMouseDoubleClick() ? 1:0 );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetMousePos)
	{
		GM_CHECK_NUM_PARAMS(0);
		v2i pos = Input::Get()->GetMousePos();
		a_thread->PushV2( v2((float)pos.x, (float)pos.y) );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetMousePosRel)
	{
		GM_CHECK_NUM_PARAMS(0);
		v2i pos = Input::Get()->GetMousePosRel();
		a_thread->PushV2( v2((float)pos.x, (float)pos.y) );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetMousePos)
	{
		int numParams = a_thread->GetNumParams();

		if( numParams == 1 )
		{
			GM_CHECK_V2_PARAM( vec, 0 );
			Input::Get()->SetMousePos( (int)vec.x, (int)vec.y );
		}
		else if ( numParams == 2 )
		{
			GM_CHECK_FLOAT_OR_INT_PARAM( x, 0 );
			GM_CHECK_FLOAT_OR_INT_PARAM( y, 0 );
			Input::Get()->SetMousePos( (int)x, (int)y );
		}
		else
		{
			return GM_EXCEPTION;
		}

		return GM_OK;
	}

	// Keyboard
	GM_MEMFUNC_DECL(IsKeyDown)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( key, 0 );
		a_thread->PushInt( Input::Get()->IsKeyDown(key) ? 1:0 );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(DidKeyJustGoDown)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( key, 0 );
		a_thread->PushInt( Input::Get()->DidKeyJustGoDown(key) ? 1:0 );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(DidKeyJustGoUp)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( key, 0 );
		a_thread->PushInt( Input::Get()->DidKeyJustGoUp(key) ? 1:0 );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(DidMouseWheelHit)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM( btn, 0 );
		a_thread->PushInt( Input::Get()->DidMouseWheelHit(btn) ? 1:0 );

		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(GetKeyPressed)
	{
		GM_CHECK_NUM_PARAMS(0);
		
		// kind of crap because it allocates string object
		// need to optimize possibly..
		
		const char str[2] = { Input::Get()->GetKeyPressed(), 0 };
		gmStringObject * str_obj = a_thread->GetMachine()->AllocStringObject(str);
		a_thread->PushString( str_obj );
		
		return GM_OK;
	}
};

static gmFunctionEntry s_gmInputLib[] = 
{
	// Mouse
	GM_LIBFUNC_ENTRY(ShowMouseCursor, Input)
	GM_LIBFUNC_ENTRY(IsMouseDown, Input)
	GM_LIBFUNC_ENTRY(DidMouseJustGoDown, Input)
	GM_LIBFUNC_ENTRY(DidMouseJustGoUp, Input)
	GM_LIBFUNC_ENTRY(GetMousePos, Input)
	GM_LIBFUNC_ENTRY(GetMousePosRel, Input)
	GM_LIBFUNC_ENTRY(SetMousePos, Input)
	GM_LIBFUNC_ENTRY(DidMouseWheelHit, Input)
	GM_LIBFUNC_ENTRY(DidMouseDoubleClick, Input)

	// Keyboard
	GM_LIBFUNC_ENTRY(IsKeyDown, Input)
	GM_LIBFUNC_ENTRY(DidKeyJustGoDown, Input)
	GM_LIBFUNC_ENTRY(DidKeyJustGoUp, Input)
	GM_LIBFUNC_ENTRY(GetKeyPressed, Input)

};

void gmBindInputLib( gmMachine * a_machine )
{
	a_machine->RegisterLibrary(s_gmInputLib, sizeof(s_gmInputLib) / sizeof(s_gmInputLib[0]), "Input" );
}

}
