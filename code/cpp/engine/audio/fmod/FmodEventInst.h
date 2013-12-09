#pragma once

#include "FmodIncludes.h"

#if defined(FMOD_ENABLED)

#include <common/HandledObj.h>
#include <gm/gmBindHeader.h>
#include <stdint.h>
#include <unordered_map>

namespace funk
{
	enum FmodPlaybackState 
	{
		PLAYBACK_PLAYING	= FMOD_STUDIO_PLAYBACK_PLAYING,
		PLAYBACK_IDLE		= FMOD_STUDIO_PLAYBACK_IDLE,
		PLAYBACK_SUSTAINING = FMOD_STUDIO_PLAYBACK_SUSTAINING,
		PLAYBACK_STOPPED	= FMOD_STUDIO_PLAYBACK_STOPPED,
	};

	enum FmodLoadingState 
	{
		LOADING_STATE_UNLOADING	= FMOD_STUDIO_LOADING_STATE_UNLOADING,
		LOADING_STATE_UNLOADED	= FMOD_STUDIO_LOADING_STATE_UNLOADED,
		LOADING_STATE_LOADING	= FMOD_STUDIO_LOADING_STATE_LOADING,
		LOADING_STATE_LOADED	= FMOD_STUDIO_LOADING_STATE_LOADED,
	};

	// fwd decl
	class FmodEventDesc;

	class FmodEventInst : public HandledObj<FmodEventInst>
	{
	private:

		FMOD_3D_ATTRIBUTES	m_3d_attribs;
		FMOD::Studio::EventInstance m_inst;

		FmodEventDesc * m_parent_desc; // the desc the spawned this!

		bool m_valid; // created?
		char m_debug_name[64]; // guid if spawned from desc. name if sub-event

		struct EventParam
		{			
			bool valid;
			FMOD::Studio::ParameterInstance inst;
		};

		std::unordered_map<uint32_t, EventParam > m_map_params;

	public:
		
		~FmodEventInst();
		
		const char * GetDebugName() const { return m_debug_name; }

		void Start();
		void Stop( bool fadeout = false );

		// params
		float GetParam( const char* a_param_name );
		bool  SetParam( const char* a_param_name, float a_val );
		bool  SetParamLerp( const char* a_param_name, float a_goal_val, float a_lerp_coeff );

		// timeline 
		int GetTimelinePos() const;
		bool SetTimelinePos( int a_pos );

		FmodPlaybackState GetPlaybackState() const;
		FmodLoadingState GetLoadingState() const;

		// 3d
		v3 GetPos();
		v3 GetVel();
		v3 GetOrientationForward();
		v3 GetOrientationUp();
		void SetPos( v3 a_pos );
		void SetVel( v3 a_vel );
		void SetOrientation( v3 a_forward, v3 a_up );

		// sub-event
		StrongHandle<FmodEventInst> CreateSubEvent( const char* a_name );

		GM_BIND_TYPEID(FmodEventInst);

	private:

		EventParam & GetEventParam( const char* a_name );

		// only event desc can init these stuff
		friend class FmodEventDesc;
		FmodEventInst();
		FMOD::Studio::EventInstance & GetInst() { return m_inst; }
		void Release();
		void Init( const char* a_debug_name, StrongHandle<FmodEventDesc> a_parent_desc );
	};

	GM_BIND_DECL( FmodEventInst );
}

#endif //FMOD_ENABLED