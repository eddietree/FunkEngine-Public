#pragma once

#include "FmodIncludes.h"

#if defined(FMOD_ENABLED)
#include <common/HandledObj.h>
#include <gm/gmBindHeader.h>
#include <gm/gmVariable.h>

#include <string>
#include <vector>

namespace funk
{
	// fwd decl
	class FmodEventInst;

	class FmodEventDesc : public HandledObj<FmodEventDesc>
	{
	private:

		FMOD::Studio::EventDescription m_desc;

		int		m_num_user_properties;
		int		m_length;
		float	m_max_distance;
		bool	m_is_oneshot;
		bool	m_is_3d;

		std::string m_guid;
		bool m_valid;

		// instances spawned from this desc
		std::vector< StrongHandle<FmodEventInst> > m_instances; 

	public:

		~FmodEventDesc();
		void Release();

		// spawns an instance of the event
		StrongHandle<FmodEventInst> SpawnInst();

		const std::string& GetGuid() const { return m_guid; }

		// will populate this table with user params
		gmVariable GetUserProperty( gmMachine* a_machine, const char* a_name );
		
		// query properties		
		float		GetUserPropertyFloat( const char * a_name ) const;
		int			GetUserPropertyInt( const char * a_name ) const;
		bool		GetUserPropertyBool( const char * a_name ) const;
		const char*	GetUserPropertyString( const char * a_name ) const;

		// query event desc data
		int		GetNumUserProperties() const { return m_num_user_properties; }
		int		GetLength() const { return m_length; }
		float	GetMaxDistance() const { return m_max_distance; }
		bool	IsOneShot() const { return m_is_oneshot; }
		bool	Is3d() const { return m_is_3d; }
		
		GM_BIND_TYPEID(FmodEventDesc);

	private:

		// constructed in the FmodSystem
		friend class FmodSystem;
		FmodEventDesc();
		void Init( const char * a_guid );
		FMOD::Studio::EventDescription & GetDesc() { return m_desc; }

		friend class FmodEventInst;
		void RegisterInst( StrongHandle<FmodEventInst> a_inst );
		void UnregisterInst( StrongHandle<FmodEventInst> a_inst );

	};

	GM_BIND_DECL( FmodEventDesc );
}

#endif //FMOD_ENABLED