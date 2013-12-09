#include "gmExtLibDebug.h"

#include "gmThread.h"
#include "gmMachine.h"

#include <gm/gmBind.h>


#include <vm/VirtualMachine.h>

// NOTE: to be replaced by gfx system
extern bool g_swap_freeze;

namespace funk
{
struct gmfConsoleLib
{
	static int GM_CDECL gmfLog(gmThread * a_thread)
	{
		GM_CHECK_STRING_PARAM(text, 0);
		GM_INT_PARAM(newLine, 1, 1);

		VirtualMachine::Get()->GetConsole().Log(text, newLine!=0 );
		return GM_OK;
	}

	static int GM_CDECL gmfEnable(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM(enable, 0);

		VirtualMachine::Get()->GetConsole().Enable(enable != 0);
		return GM_OK;
	}

	static int GM_CDECL gmfIsEnabled(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(0);
		a_thread->PushInt( VirtualMachine::Get()->GetConsole().IsEnabled() );
		return GM_OK;
	}


};

struct gmfDebugLib
{
	static int GM_CDECL gmfBreak(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(0);

		 gmDebugSession * session = (gmDebugSession *)VirtualMachine::Get()->GetVM().m_debugUser;

		 if ( session )
		 {
			 session->m_owner->BreakIntoRunningThread();
		 }

		return GM_OK;
	}

	static int GM_CDECL gmfSetSwapFreeze(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM(swap_freeze, 0);

		g_swap_freeze = swap_freeze != 0;

		return GM_OK;
	}
};

static gmFunctionEntry s_gmConsoleLib[] = 
{
	GM_LIBFUNC_ENTRY(Log, Console)
	GM_LIBFUNC_ENTRY(Enable, Console)
	GM_LIBFUNC_ENTRY(IsEnabled, Console)
};

static gmFunctionEntry s_gmDebugLib[] = 
{
	GM_LIBFUNC_ENTRY(Break, Debug)
	GM_LIBFUNC_ENTRY(SetSwapFreeze, Debug)
};

void gmBindFunkDebugLib( gmMachine * a_machine )
{
	a_machine->RegisterLibrary(s_gmConsoleLib, sizeof(s_gmConsoleLib) / sizeof(s_gmConsoleLib[0]), "Console" );
	a_machine->RegisterLibrary(s_gmDebugLib, sizeof(s_gmDebugLib) / sizeof(s_gmDebugLib[0]), "Debug" );
}

}
