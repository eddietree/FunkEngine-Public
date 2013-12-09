#pragma once

#include <audio/fmod/FmodIncludes.h>

class gmMachine;

namespace funk
{
	#if defined(FMOD_ENABLED)
	void gmBindFmodLib( gmMachine * a_machine );
	#else
	inline void gmBindFmodLib( gmMachine * a_machine ){}
	#endif

}