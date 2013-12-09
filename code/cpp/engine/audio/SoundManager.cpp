#include "SoundManager.h"

#include <common/Util.h>
#include <common/Debug.h>
#include <imgui/Imgui.h>

namespace funk
{
	SoundManager::SoundManager() 
	{
		// Device
		m_device = alcOpenDevice(NULL);
		ASSERT(m_device);
		CheckError();

		// Context
		m_ctx = alcCreateContext(m_device, NULL);
		ASSERT(m_ctx);
		alcMakeContextCurrent(m_ctx);
		CheckError();

		const float LISTENER_METERS_PER_UNIT = 0.3f;
		// TODO_MAC: alListenerf(AL_METERS_PER_UNIT, LISTENER_METERS_PER_UNIT);
		CheckError();

		TRACE("Initiated OpenAL Version %x.%x\n", ALC_MAJOR_VERSION, ALC_MINOR_VERSION );
	}
	
	SoundManager::~SoundManager()
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_ctx);
		alcCloseDevice(m_device);

		m_device = nullptr;
	}

	void SoundManager::Update( float a_dt )
	{
		// TODO: sort thru Sources and bring them to the pool if finished
	}

	void SoundManager::CheckError()
	{
		#if !defined(FUNK_FINAL)
		ALCenum al_err = alcGetError(m_device);
		CHECK(al_err == ALC_NO_ERROR, "OpenAL error: %d", al_err );
		#endif
	}

	v3 SoundManager::GetListenerPos()
	{
		float pos[3];
		alGetListenerfv( AL_POSITION, pos );
		return v3( pos[0], pos[1], pos[2] );
	}

	v3 SoundManager::GetListenerVel()
	{
		float vel[3];
		alGetListenerfv( AL_VELOCITY, vel );
		return v3( vel[0], vel[1], vel[2] );
	}

	v3 SoundManager::GetListenerOrientationForward()
	{
		float orient[6];
		alGetListenerfv( AL_ORIENTATION, orient );
		return v3( orient[0], orient[1], orient[2] );
	}

	v3 SoundManager::GetListenerOrientationUp()
	{
		float orient[6];
		alGetListenerfv( AL_ORIENTATION, orient );
		return v3( orient[3], orient[4], orient[5] );
	}

	void SoundManager::SetListenerPos( v3 a_pos )
	{
		alListener3f( AL_POSITION, a_pos.x, a_pos.y, a_pos.z );
	}
	
	void SoundManager::SetListenerVel( v3 a_vel )
	{
		alListener3f( AL_VELOCITY, a_vel.x, a_vel.y, a_vel.z );
	}

	void SoundManager::SetListenerOrientation( v3 a_forward, v3 a_up )
	{
		float orient[6] = 
		{
			a_forward.x, a_forward.y, a_forward.z, 
			a_up.x, a_up.y, a_up.z 
		};

		alListenerfv( AL_ORIENTATION, orient );
	}

	void SoundManager::SetVolume( float a_vol )
	{
		alListenerf( AL_GAIN, a_vol );
	}
}