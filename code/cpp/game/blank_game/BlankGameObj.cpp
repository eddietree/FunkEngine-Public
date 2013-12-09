#include "BlankGameObj.h"

#include <gm/gmBind.h>

namespace blank_game
{

using namespace funk;

BlankGameObj::BlankGameObj()
{
		
}

GM_REG_NAMESPACE(BlankGameObj)
{
	GM_MEMFUNC_DECL(CreateBlankGameObj)
	{
		GM_CHECK_NUM_PARAMS(0);
		GM_PUSH_USER_HANDLED( BlankGameObj, new BlankGameObj() );
		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(Foo)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM( a_val, 0 );
		
		GM_GET_THIS_PTR( BlankGameObj, ptr );
		a_thread->PushInt( ptr->Foo(a_val) );
		
		return GM_OK;
	}
	
	GM_GEN_MEMFUNC_VOID_VOID( BlankGameObj, Update )
	GM_GEN_MEMFUNC_VOID_VOID( BlankGameObj, Draw )
}

GM_REG_MEM_BEGIN(BlankGameObj)
	GM_REG_MEMFUNC( BlankGameObj, Update )
	GM_REG_MEMFUNC( BlankGameObj, Draw )
	GM_REG_MEMFUNC( BlankGameObj, Foo )
GM_REG_HANDLED_DESTRUCTORS(BlankGameObj)
GM_REG_MEM_END()
GM_BIND_DEFINE(BlankGameObj)

}
