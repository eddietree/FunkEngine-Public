#pragma once

#include <unordered_map>

#include <common/Singleton.h>
#include <math/v3.h>

#include "FmodIncludes.h"
#include "FmodEventDesc.h"
#include "FmodFFT.h"

#if defined(FMOD_ENABLED)

namespace funk
{
    class FmodSystem : public Singleton<FmodSystem>
	{
	private:
		FMOD::Studio::System m_system;
		FMOD_3D_ATTRIBUTES	 m_listener_attribs;
		std::unordered_map<uint32_t, FMOD::Studio::Bank> m_map_banks; // hash(filename) -> Bank
		FmodFFT m_fft;

	public:

		// banks and events
		void LoadBank( const char* a_file );
		void UnloadBank( const char* a_file );
		void UnloadAllBanks();
		StrongHandle<FmodEventDesc> CreateEventDesc(const char * a_event_guid);

		void Update( float a_dt );
		void Gui();

		void SetVolume( float a_vol );

		// listener
		v3 GetListenerPos();
		v3 GetListenerVel();
		v3 GetListenerOrientationForward();
		v3 GetListenerOrientationUp();
		void SetListenerPos( v3 a_pos );
		void SetListenerVel( v3 a_vel );
		void SetListenerOrientation( v3 a_forward, v3 a_up );

		float GetValFFT( int a_bin_index ) const;

    private:

        friend Singleton<FmodSystem>;
		FmodSystem();
		~FmodSystem();
    };
}

#endif // FMOD_ENABLED