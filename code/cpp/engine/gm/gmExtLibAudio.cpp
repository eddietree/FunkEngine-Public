#include "gmExtLibAudio.h"

#include "gmThread.h"
#include "gmMachine.h"
#include "gmHelpers.h"
#include <gm/gmBind.h>

#include <common/Debug.h>
#include <audio/SoundManager.h>

namespace funk
{

struct gmfAudioLib
{	
	GM_MEMFUNC_DECL(GetListenerPos)
	{
		a_thread->PushV3( SoundManager::Ref().GetListenerPos() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetListenerVel)
	{
		a_thread->PushV3( SoundManager::Ref().GetListenerPos() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetListenerOrientationForward)
	{
		a_thread->PushV3( SoundManager::Ref().GetListenerPos() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetListenerOrientationUp)
	{
		a_thread->PushV3( SoundManager::Ref().GetListenerPos() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetListenerPos)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_V3_PARAM( a_val, 0 );
		SoundManager::Ref().SetListenerPos(a_val);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetListenerVel)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_V3_PARAM( a_val, 0 );
		SoundManager::Ref().SetListenerVel(a_val);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetListenerOrientation)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V3_PARAM( a_val_0, 0 );
		GM_CHECK_V3_PARAM( a_val_1, 1 );

		SoundManager::Ref().SetListenerOrientation(a_val_0, a_val_1);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetVolume)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_FLOAT_PARAM( a_vol, 0 );		

		SoundManager::Ref().SetVolume(a_vol);
		return GM_OK;
	}
};

static gmFunctionEntry s_gmAudioLib[] = 
{	
	GM_LIBFUNC_ENTRY(GetListenerPos, Audio)
	GM_LIBFUNC_ENTRY(GetListenerVel, Audio)
	GM_LIBFUNC_ENTRY(GetListenerOrientationForward, Audio)
	GM_LIBFUNC_ENTRY(GetListenerOrientationUp, Audio)
	GM_LIBFUNC_ENTRY(SetListenerPos, Audio)
	GM_LIBFUNC_ENTRY(SetListenerVel, Audio)
	GM_LIBFUNC_ENTRY(SetListenerOrientation, Audio)
	GM_LIBFUNC_ENTRY(SetVolume, Audio)
};

void gmBindAudioLib( gmMachine * a_machine )
{
	a_machine->RegisterLibrary(s_gmAudioLib, sizeof(s_gmAudioLib) / sizeof(s_gmAudioLib[0]), "Audio" );
}

}