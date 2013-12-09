#include "FmodSystem.h"

#if defined(FMOD_ENABLED)

#include <common/Util.h>
#include <common/Debug.h>
#include <imgui/Imgui.h>
#include <system/FileUtil.h>

namespace funk
{
	const int MAX_CHANNELS = 64;

	FmodSystem::FmodSystem() 
	{
		void *extra_driver_data = 0;

		FMOD_ERROR_CHECK( FMOD::Studio::System::create(&m_system) );
		FMOD_ERROR_CHECK( m_system.initialize(MAX_CHANNELS, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extra_driver_data) );

		LOG("FmodSystem initialized with %d channels", MAX_CHANNELS);

		m_system.getListenerAttributes(&m_listener_attribs);

		// get low level system
		FMOD::System *sys_low_level = nullptr;
		FMOD_ERROR_CHECK( m_system.getLowLevelSystem( &sys_low_level ) );
		ASSERT( sys_low_level != nullptr );
		m_fft.Init( sys_low_level );
	}

	void FmodSystem::UnloadAllBanks()
	{
		for ( auto& it : m_map_banks )
		{
			FMOD::Studio::Bank & bank = it.second;
			bank.unloadSampleData();

			FMOD_STUDIO_LOADING_STATE loading_state;
			do
			{
				bank.getSampleLoadingState( &loading_state );
			} while( loading_state != FMOD_STUDIO_LOADING_STATE_UNLOADED );

			bank.unload();
		}

		m_map_banks.clear();
	}
	
	FmodSystem::~FmodSystem()
	{
		UnloadAllBanks();
		m_fft.Release();
		FMOD_ERROR_CHECK( m_system.release() );
	}

	void FmodSystem::LoadBank( const char* a_file )
	{
		uint32_t hash_key = HashString(a_file);

		// if exists, unload
		auto it = m_map_banks.find(hash_key);
		if ( it != m_map_banks.end() )
		{
			UnloadBank( a_file );
		}

		FMOD::Studio::Bank &bank = m_map_banks[ hash_key ];
        
        const char* full_path = GetFullPath(a_file);
		FMOD_ERROR_CHECK( m_system.loadBank(full_path, &bank) );
		LOG("FMOD loaded bank %s", full_path);
	}

	void FmodSystem::UnloadBank( const char* a_file )
	{
		uint32_t hash_key = HashString(a_file);

		// if exists, unload
		auto it = m_map_banks.find(hash_key);
		if ( it != m_map_banks.end() )
		{
			FMOD::Studio::Bank &bank = it->second;
			FMOD_ERROR_CHECK( bank.unload() );
			m_map_banks.erase( it );
		}
	}

	StrongHandle<FmodEventDesc> FmodSystem::CreateEventDesc( const char * a_event_guid )
	{
		ASSERT(a_event_guid != nullptr);

		// build id
		FMOD::Studio::ID event_id = {0};
		{
			FMOD_RESULT fmod_result = FMOD::Studio::parseID(a_event_guid, &event_id);

			if ( fmod_result != FMOD_OK )
			{
				WARN( false, "FMOD unable to read event GUID '%s' : %s", a_event_guid, FMOD_ErrorString(fmod_result) );
				return nullptr;
			}
		}

		// build the desc
		StrongHandle<FmodEventDesc> result = new FmodEventDesc;
		FMOD::Studio::EventDescription & desc = result->GetDesc();

		FMOD_RESULT fmod_result = m_system.getEvent(&event_id, FMOD_STUDIO_LOAD_BEGIN_NOW, &desc);
		if ( fmod_result != FMOD_OK )
		{
			WARN( fmod_result == FMOD_OK, "FMOD unable to create event '%s' : %s", a_event_guid, FMOD_ErrorString(fmod_result) );
			return nullptr;
		}

		result->Init(a_event_guid);
		return result;
	}

	void FmodSystem::Update( float a_dt )
	{
		FMOD_ERROR_CHECK( m_system.update() );
		m_fft.Update();
	}

	void FmodSystem::SetVolume( float a_vol )
	{
		// get low level system
		FMOD::System *sys_low_level = nullptr;
		FMOD_ERROR_CHECK( m_system.getLowLevelSystem( &sys_low_level ) );

		// master group
		FMOD::ChannelGroup *master_channel_group = nullptr;
		FMOD_ERROR_CHECK( sys_low_level->getMasterChannelGroup( &master_channel_group ) );
		ASSERT( master_channel_group != nullptr );

		master_channel_group->setVolume( a_vol );
	}

	v3 FmodSystem::GetListenerPos()
	{
		return toV3( m_listener_attribs.position );
	}

	v3 FmodSystem::GetListenerVel()
	{
		return toV3( m_listener_attribs.velocity );
	}

	v3 FmodSystem::GetListenerOrientationForward()
	{
		return toV3( m_listener_attribs.forward );
	}

	v3 FmodSystem::GetListenerOrientationUp()
	{
		return toV3( m_listener_attribs.up );
	}

	void FmodSystem::SetListenerPos( v3 a_pos )
	{
		m_listener_attribs.position = toVECTOR(a_pos);
		m_system.setListenerAttributes( &m_listener_attribs );
	}
	
	void FmodSystem::SetListenerVel( v3 a_vel )
	{
		m_listener_attribs.velocity = toVECTOR(a_vel);
		m_system.setListenerAttributes( &m_listener_attribs );
	}

	void FmodSystem::SetListenerOrientation( v3 a_forward, v3 a_up )
	{
		m_listener_attribs.forward = toVECTOR(a_forward);
		m_listener_attribs.up = toVECTOR(a_up);
		m_system.setListenerAttributes( &m_listener_attribs );
	}

	float FmodSystem::GetValFFT( int a_bin_index ) const
	{
		return m_fft.GetVal(a_bin_index);
	}

	void FmodSystem::Gui()
	{
		static bool show_gui_cpu_details = false;
		
		FMOD::System * sys;
		FMOD_ERROR_CHECK( m_system.getLowLevelSystem(&sys) );
		
		int num_channels = 0;
		int memory_used, memory_alloced;
		float cpu_usage_dsp = 0.0f;
		float cpu_usage_stream = 0.0f;
		float cpu_usage_geometry = 0.0f;
		float cpu_usage_update = 0.0f;
		float cpu_usage_total = 0.0f;

		sys->getChannelsPlaying( &num_channels );
		sys->getCPUUsage( &cpu_usage_dsp, &cpu_usage_stream, &cpu_usage_geometry, &cpu_usage_update, &cpu_usage_total );
		FMOD::Memory_GetStats( &memory_used, &memory_alloced, false );

		Imgui::Header("Fmod");

		Imgui::FillBarInt( "Channels Playing", num_channels, 0, 64 );
		Imgui::FillBarInt( "Memory Used (Bytes)", memory_used, 0, memory_alloced);
		Imgui::FillBarFloat( "CPU Usage Total", cpu_usage_total, 0.0f, 100.0f );

		// more cpu usage
		show_gui_cpu_details = Imgui::CheckBox( "More CPU Usage", show_gui_cpu_details );
		if ( show_gui_cpu_details )
		{
			Imgui::FillBarFloat( "CPU Usage DSP", cpu_usage_dsp, 0.0f, 100.0f );
			Imgui::FillBarFloat( "CPU Usage Stream", cpu_usage_stream, 0.0f, 100.0f );
			Imgui::FillBarFloat( "CPU Usage Geometry", cpu_usage_geometry, 0.0f, 100.0f );
			Imgui::FillBarFloat( "CPU Usage Update", cpu_usage_update, 0.0f, 100.0f );
		}

		m_fft.Gui();
	}
}

#endif // FMOD_ENABLED