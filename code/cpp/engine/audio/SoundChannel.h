#pragma once

#include <common/HandledObj.h>
#include <gm/gmBindHeader.h>
#include <math/v3.h>

#include "SoundBuffer.h"

// Reference OpenAL 1.1 specs: http://connect.creativelabs.com/openal/Documentation/OpenAL%201.1%20Specification.pdf

namespace funk
{
	enum SoundChannelState
	{
		SOUNDSOURCE_INITIAL	= 0, 
		SOUNDSOURCE_PLAYING	= 1,
		SOUNDSOURCE_STOPPED	= 2,
		SOUNDSOURCE_PAUSED	= 3,

		SOUNDSOURCE_NULL	= 4
	};

	/*
		SoundChannels are "sources" that emit sound
		but you must first bind a SoundBuffer to this source
		in order for the Listener to hear the sound
	*/

	class SoundChannel : public HandledObj<SoundChannel>
	{
	private:
		unsigned int m_id;

		// buffer that is bound
		StrongHandle<SoundBuffer> m_buffer; 

	public:

		SoundChannel();
		~SoundChannel();
		void Init();

		unsigned int GetId() const { return m_id; }
		SoundChannelState GetState() const;

		// buffer
		void BindBuffer( StrongHandle<SoundBuffer> a_buffer );
		void UnbindBuffer();

		// functions used when sound source is recycled
		void Reset();
		bool IsActive() const; // check to see if playing/puased		

		// playing
		void Play(); // rewinds if already playing
		void Stop();
		void SetPause( bool pause );
		bool IsPlaying() const;
		bool IsLooping() const;
		void SetLooping( bool a_loop );

		// effects
		float	GetVolume() const;
		void	SetVolume(float vol); // default 1, range [0,any]
		float	GetPitch() const;
		void	SetPitch(float a_pitch); // range: [0.5-2.0], default: 1.0
		float	GetPan() const; // for 2d audio
		void	SetPan(float pan); // for 2d audio, range [-1,1]
		
		// positioning attributes
		// if "relative", pos/vel/direction props of src relative to listener
		void SetRelativeToListener( bool a_relative );
		bool GetRelativeToListener() const;

		// physics
		v3		GetPos() const;
		void	SetPos( v3 a_pos );
		v3		GetVel() const;
		void	SetVel( v3 a_vel );

		// distance model
		// TODO  AL_REFERENCE_DISTANCE, AL_ROLLOFF_FACTOR, AL_MAX_DISTANCE

		// direction and cone
		v3		GetDirection() const;
		void	SetDirection( v3 a_dir );
		float	GetConeInnerAngle(); // radians
		void	SetConeInnerAngle( float a_radians );
		float	GetConeOuterAngle();  // radians
		void	SetConeOuterAngle( float a_radians );
		float	GetConeOuterGain();
		void	SetConeOuterGain( float a_gain );

		// sound position
		float GetCurrPosSeconds() const;
		void SetPosSeconds( float a_seconds );
		
		GM_BIND_TYPEID(SoundChannel);

	private:

		void Release();		
	};

	GM_BIND_DECL( SoundChannel );
}