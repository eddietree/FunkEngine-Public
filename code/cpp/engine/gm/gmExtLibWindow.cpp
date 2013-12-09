#include "gmExtLibWindow.h"

#include "gmThread.h"
#include "gmMachine.h"
#include "gmHelpers.h"

#include <gm/gmBind.h>
#include <math/v2.h>

#include <system/Window.h>

namespace funk
{
struct gmfWindowLib
{
	GM_MEMFUNC_DECL(Dimen)
	{
		GM_CHECK_NUM_PARAMS(0);
		a_thread->PushV2( Window::Get()->Sizef() );

		return GM_OK;
	}
	
	GM_MEMFUNC_DECL(MonitorDimen)
	{
		GM_CHECK_NUM_PARAMS(0);
		a_thread->PushV2( Window::Get()->MonitorSizef() );
		
		return GM_OK;
	}

    GM_MEMFUNC_DECL(IsFullScreen)
    {
        GM_CHECK_NUM_PARAMS(0);

        const bool is_full_screen = Window::Get()->IsFullScreen();

        a_thread->PushInt(is_full_screen != 0);
        return GM_OK;
    }

    GM_MEMFUNC_DECL(SetTitle)
    {
        GM_CHECK_NUM_PARAMS(1);
        GM_CHECK_STRING_PARAM(title, 0);

        Window::Get()->SetTitle(title);
        return GM_OK;
    }

	GM_MEMFUNC_DECL(ShowMouseCursor)
    {
        GM_CHECK_NUM_PARAMS(1);
        GM_CHECK_INT_PARAM(a_enable, 0);

		Window::Get()->ShowMouseCursor(a_enable != 0);
        return GM_OK;
    }

	GM_MEMFUNC_DECL(Exit)
    {
        GM_CHECK_NUM_PARAMS(0);
		Window::Get()->Exit();
        return GM_OK;
    }
};

static gmFunctionEntry s_gmWindowLib[] = 
{
	GM_LIBFUNC_ENTRY(Dimen, Window)
	GM_LIBFUNC_ENTRY(MonitorDimen, Window)
	GM_LIBFUNC_ENTRY(IsFullScreen, Window)
	GM_LIBFUNC_ENTRY(SetTitle, Window)
	GM_LIBFUNC_ENTRY(ShowMouseCursor, Window)
	GM_LIBFUNC_ENTRY(Exit, Window)
};

void gmBindWindowLib( gmMachine * a_machine )
{
	a_machine->RegisterLibrary(s_gmWindowLib, sizeof(s_gmWindowLib) / sizeof(s_gmWindowLib[0]), "Window" );
}

}