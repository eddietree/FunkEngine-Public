#include "gmExtLibFmod.h"

#if defined(FMOD_ENABLED)

#include "gmThread.h"
#include "gmMachine.h"
#include "gmHelpers.h"
#include <gm/gmBind.h>

#include <common/Debug.h>
#include <audio/fmod/FmodSystem.h>

namespace funk
{

struct gmfFmodLib
{	
	GM_MEMFUNC_DECL(LoadBank)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( a_file, 0 );

		FmodSystem::Ref().LoadBank( a_file );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(UnloadBank)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( a_file, 0 );

		FmodSystem::Ref().UnloadBank( a_file );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(UnloadAllBanks)
	{
		GM_CHECK_NUM_PARAMS(0);
		FmodSystem::Ref().UnloadAllBanks();
		return GM_OK;
	}

	GM_MEMFUNC_DECL(CreateEventDesc)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( a_event_guid, 0 );

		StrongHandle<FmodEventDesc> event_desc = FmodSystem::Ref().CreateEventDesc( a_event_guid );
		
		if ( event_desc == nullptr ) a_thread->PushNull();
		else { GM_PUSH_USER_HANDLED( FmodEventDesc, event_desc.Get() );}

		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetListenerPos)
	{
		a_thread->PushV3( FmodSystem::Ref().GetListenerPos() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetListenerVel)
	{
		a_thread->PushV3( FmodSystem::Ref().GetListenerPos() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetListenerOrientationForward)
	{
		a_thread->PushV3( FmodSystem::Ref().GetListenerPos() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetListenerOrientationUp)
	{
		a_thread->PushV3( FmodSystem::Ref().GetListenerPos() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetListenerPos)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_V3_PARAM( a_val, 0 );
		FmodSystem::Ref().SetListenerPos(a_val);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetListenerVel)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_V3_PARAM( a_val, 0 );
		FmodSystem::Ref().SetListenerVel(a_val);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetListenerOrientation)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_V3_PARAM( a_val_0, 0 );
		GM_CHECK_V3_PARAM( a_val_1, 1 );

		FmodSystem::Ref().SetListenerOrientation(a_val_0, a_val_1);
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetValFFT)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_INT_PARAM( a_bin_index, 0 );		

		float val = FmodSystem::Ref().GetValFFT(a_bin_index);
		a_thread->PushFloat( val );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetVolume)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_FLOAT_PARAM( a_vol, 0 );		

		FmodSystem::Ref().SetVolume(a_vol);
		return GM_OK;
	}
};

static gmFunctionEntry s_gmFmodLib[] = 
{	
	GM_LIBFUNC_ENTRY(LoadBank, Fmod)
	GM_LIBFUNC_ENTRY(UnloadBank, Fmod)
	GM_LIBFUNC_ENTRY(UnloadAllBanks, Fmod)
	GM_LIBFUNC_ENTRY(CreateEventDesc, Fmod)
	GM_LIBFUNC_ENTRY(GetListenerPos, Fmod)
	GM_LIBFUNC_ENTRY(GetListenerVel, Fmod)
	GM_LIBFUNC_ENTRY(GetListenerOrientationForward, Fmod)
	GM_LIBFUNC_ENTRY(GetListenerOrientationUp, Fmod)
	GM_LIBFUNC_ENTRY(SetListenerPos, Fmod)
	GM_LIBFUNC_ENTRY(SetListenerVel, Fmod)
	GM_LIBFUNC_ENTRY(SetListenerOrientation, Fmod)
	GM_LIBFUNC_ENTRY(GetValFFT, Fmod)
	GM_LIBFUNC_ENTRY(SetVolume, Fmod)
};

void gmBindFmodLib( gmMachine * a_machine )
{
	a_machine->RegisterLibrary(s_gmFmodLib, sizeof(s_gmFmodLib) / sizeof(s_gmFmodLib[0]), "Fmod" );
}

}

#endif // FMOD_ENABLED