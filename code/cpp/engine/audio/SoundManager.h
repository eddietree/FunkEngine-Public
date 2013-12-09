
#pragma once

#include <common/Singleton.h>
#include <audio/ALIncludes.h>
#include <math/v3.h>

namespace funk
{
    class SoundManager : public Singleton<SoundManager>
	{
	private:
		ALCdevice* m_device;
		ALCcontext* m_ctx;

	public:
		void CheckError();
		void Update( float a_dt );

		// listener
		v3 GetListenerPos();
		v3 GetListenerVel();
		v3 GetListenerOrientationForward();
		v3 GetListenerOrientationUp();
		void SetListenerPos( v3 a_pos );
		void SetListenerVel( v3 a_vel );
		void SetListenerOrientation( v3 a_forward, v3 a_up );
		void SetVolume( float a_vol ); // [0,1]

    private:
        friend Singleton<SoundManager>;
		SoundManager();
		~SoundManager();
    };
}
