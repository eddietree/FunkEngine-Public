#include "gmBlankGameInit.h"

#include <gm/gmMachine.h>
#include <gm/gmBind.h>

#include "BlankGameObj.h"

namespace blank_game
{
	void InitGameLibs( gmMachine * a_machine )
	{
		GM_BIND_INIT( BlankGameObj, a_machine );
	}
}