#include "gmExtLibProfiler.h"

#include "gmThread.h"
#include "gmMachine.h"
#include "gmHelpers.h"

#include <gm/gmBind.h>

#include <common/Profiler.h>

namespace funk
{
struct gmfProfilerLib
{
	GM_MEMFUNC_DECL(Begin)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( a_entry_name, 0 );

		PROFILE_BEGIN( a_entry_name );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(End)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( a_entry_name, 0 );

		PROFILE_END( a_entry_name );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Gui)
	{
		GM_CHECK_NUM_PARAMS(0);

		if ( Profiler::InstExists() ) Profiler::Ref().Gui();

		return GM_OK;
	}

	GM_MEMFUNC_DECL(GuiLineGraphs)
	{
		GM_CHECK_NUM_PARAMS(0);
		if ( Profiler::InstExists() ) Profiler::Ref().GuiLineGraphs();
		return GM_OK;
	}

	GM_MEMFUNC_DECL(HasLineGraphs)
	{
		GM_CHECK_NUM_PARAMS(0);
		if ( Profiler::InstExists() ) a_thread->PushBool( Profiler::Ref().HasLineGraphs() );
		else a_thread->PushBool( false );
		return GM_OK;
	}
};

static gmFunctionEntry s_gmProfilerLib[] = 
{
	GM_LIBFUNC_ENTRY(Begin, Profiler)
	GM_LIBFUNC_ENTRY(End, Profiler)
	GM_LIBFUNC_ENTRY(Gui, Profiler)
	GM_LIBFUNC_ENTRY(GuiLineGraphs, Profiler)
	GM_LIBFUNC_ENTRY(HasLineGraphs, Profiler)
};

void gmBindProfilerLib( gmMachine * a_machine )
{
	a_machine->RegisterLibrary(s_gmProfilerLib, sizeof(s_gmProfilerLib) / sizeof(s_gmProfilerLib[0]), "Profiler" );
}

}
