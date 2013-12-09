#pragma once

#include <system/Config.h>

#if !PLATFORM_MOBILE
    #define FMOD_ENABLED
#endif

#if defined(FMOD_ENABLED)

#include <fmod/fmod.hpp>
#include <fmod/fmod_studio.hpp>
#include <fmod/fmod_errors.h>

#include <common/Debug.h>
#include <math/v3.h>

inline void FMOD_ERROR_CHECK( FMOD_RESULT a_result )
{
	//#if !defined(FUNK_FINAL)
	if (a_result != FMOD_OK)
	{
		LOG_ERROR("FMOD Error %s", FMOD_ErrorString(a_result) );
		ERROR_MSG("FMOD error %d - %s", a_result, FMOD_ErrorString(a_result));
	}
	//#endif
}

inline funk::v3 toV3( FMOD_VECTOR a_vec ) 
{ 
	return funk::v3(a_vec.x, a_vec.y, a_vec.z); 
}

inline FMOD_VECTOR toVECTOR( funk::v3 a_vec ) 
{ 
	FMOD_VECTOR result;
	result.x = a_vec.x;
	result.y = a_vec.y;
	result.z = a_vec.z;
	return result;
}


#endif