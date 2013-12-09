#include "FmodEventInst.h"

#if defined(FMOD_ENABLED)

#include "FmodSystem.h"

#include "FmodEventDesc.h"

#include <gm/gmBind.h>
#include <common/Debug.h>
#include <common/Util.h>

namespace funk
{
FmodEventInst::FmodEventInst() : m_valid(false), m_parent_desc(nullptr)
{
}

FmodEventInst::~FmodEventInst()
{
	Release();

	if ( m_parent_desc != nullptr )
	{
		m_parent_desc->UnregisterInst(this);
		m_parent_desc = nullptr;
	}
}

void FmodEventInst::Release()
{
	if ( m_valid )
	{
		TRACE( "FmodEventInst::Release() - %s\n", m_debug_name );

		// FMOD Inst must be destructed before the Desc
		FMOD_ERROR_CHECK( m_inst.stop( FMOD_STUDIO_STOP_IMMEDIATE ) );
		FMOD_ERROR_CHECK( m_inst.release() );

		m_valid = false;
		m_parent_desc = nullptr;
	}
}

void FmodEventInst::Init( const char* a_debug_name, StrongHandle<FmodEventDesc> a_parent_desc )
{
	TRACE( "FmodEventInst::Init() - %s\n", a_debug_name );

	FMOD_ERROR_CHECK( m_inst.get3DAttributes( &m_3d_attribs ) );

	// future: grab all parameters and store them into map

	m_parent_desc = a_parent_desc;
	m_valid = true;

	const int len = strlen(a_debug_name);
	ASSERT( len < sizeof(m_debug_name) );

	memcpy( m_debug_name, a_debug_name, len);
	m_debug_name[len] = '\0';
}

void FmodEventInst::Start()
{
	if (!m_valid) return;
	FMOD_ERROR_CHECK( m_inst.start() );
}

void FmodEventInst::Stop( bool fadeout )
{
	if (!m_valid) return;
	FMOD_STUDIO_STOP_MODE mode = fadeout ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;
	FMOD_ERROR_CHECK( m_inst.stop(mode) );
}

FmodEventInst::EventParam & FmodEventInst::GetEventParam( const char* a_name )
{
	ASSERT(a_name != nullptr);

	uint32_t hash_int = HashString(a_name);
	auto it = m_map_params.find(hash_int);

	// not found, create new inst
	if ( it == m_map_params.end() )
	{
		// cap on max number of parameters created
		const int MAX_NUM_PARAMS = 32;
		ASSERT( m_map_params.size() <= MAX_NUM_PARAMS );

		// insert into map
		auto result = m_map_params.insert( std::make_pair( hash_int, EventParam() ) );
		FmodEventInst::EventParam & param = result.first->second;
		param.valid = false;

		// if valid parameter in fmod..
		FMOD_RESULT fmod_result = m_inst.getParameter( a_name, &param.inst );
		if ( fmod_result == FMOD_OK )
		{
			param.valid = true;

			// grab range min/max (currently not available in API)
		}

		WARN( fmod_result == FMOD_OK, "FMOD FmodEventInst (%s): unable to get param '%s'", m_debug_name, a_name );
		return param;
	}

	// has been found!
	return it->second;
}

float FmodEventInst::GetParam( const char* a_param_name )
{
	EventParam & param = GetEventParam( a_param_name );
	if( !param.valid ) return 0.0f;

	float value = 0.0f;
	FMOD_ERROR_CHECK( param.inst.getValue( &value ) );
	return value;
}

bool FmodEventInst::SetParam( const char* a_param_name, float a_val )
{
	if (!m_valid) return false;

	EventParam & param = GetEventParam( a_param_name );
	if( !param.valid ) return false;

	FMOD_ERROR_CHECK( param.inst.setValue( a_val ) );
	return true;
}

bool FmodEventInst::SetParamLerp( const char* a_param_name, float a_goal_val, float a_lerp_coeff )
{
	float curr_val = GetParam( a_param_name );
	float new_val = lerp( curr_val, a_goal_val, a_lerp_coeff );
	return SetParam( a_param_name, new_val );
}

int FmodEventInst::GetTimelinePos() const
{
	if (!m_valid) return 0;

	int pos;
	FMOD_ERROR_CHECK(m_inst.getTimelinePosition(&pos));
	return pos;
}

bool FmodEventInst::SetTimelinePos( int a_pos )
{
	if (!m_valid) return false;

	FMOD_ERROR_CHECK( m_inst.setTimelinePosition( a_pos ) );
	return true;
}

FmodPlaybackState FmodEventInst::GetPlaybackState() const
{
	FMOD_STUDIO_PLAYBACK_STATE playback_state;
	FMOD_ERROR_CHECK( m_inst.getPlaybackState( &playback_state ) );
	return (FmodPlaybackState)playback_state;
}

FmodLoadingState FmodEventInst::GetLoadingState() const
{
	FMOD_STUDIO_LOADING_STATE loading_state;
	FMOD_ERROR_CHECK( m_inst.getLoadingState( &loading_state ) );
	return (FmodLoadingState)loading_state;
}

v3 FmodEventInst::GetPos()
{
	return toV3(m_3d_attribs.position);
}

v3 FmodEventInst::GetVel()
{
	return toV3(m_3d_attribs.velocity);
}

v3 FmodEventInst::GetOrientationForward()
{
	return toV3(m_3d_attribs.forward);
}

v3 FmodEventInst::GetOrientationUp()
{
	return toV3(m_3d_attribs.up);
}

void FmodEventInst::SetPos( v3 a_pos )
{
	m_3d_attribs.position = toVECTOR( a_pos );
	FMOD_ERROR_CHECK( m_inst.set3DAttributes( &m_3d_attribs ) );
}

void FmodEventInst::SetVel( v3 a_vel )
{
	m_3d_attribs.velocity = toVECTOR( a_vel );
	FMOD_ERROR_CHECK( m_inst.set3DAttributes( &m_3d_attribs ) );
}

void FmodEventInst::SetOrientation( v3 a_forward, v3 a_up )
{
	m_3d_attribs.forward = toVECTOR( a_forward );
	m_3d_attribs.up = toVECTOR( a_up );
	FMOD_ERROR_CHECK( m_inst.set3DAttributes( &m_3d_attribs ) );
}

StrongHandle<FmodEventInst> FmodEventInst::CreateSubEvent( const char* a_name )
{
	if (!m_valid) return nullptr;

	StrongHandle<FmodEventInst> result = new FmodEventInst;
	FMOD::Studio::EventInstance & result_inst = result->GetInst();
	
	// create the sub event
	FMOD_ERROR_CHECK( m_inst.createSubEvent( a_name, &result_inst ) );

	result->Init( a_name, nullptr );
	return result;
}

GM_REG_NAMESPACE(FmodEventInst)
{
	GM_MEMFUNC_CONSTRUCTOR(FmodEventInst)
	{
		ERROR_MSG("FmodEventInst can only be constructed via FmodEventDesc.SpawnInst()");
		return GM_EXCEPTION;
	}

	GM_MEMFUNC_DECL(CreateSubEvent)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( a_name, 0);
		GM_GET_THIS_PTR(FmodEventInst, ptr);

		StrongHandle<FmodEventInst> result = ptr->CreateSubEvent(a_name);
		GM_PUSH_USER_HANDLED( FmodEventInst, result.Get() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetParam)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( a_name, 0);
		GM_GET_THIS_PTR(FmodEventInst, ptr);

		a_thread->PushFloat( ptr->GetParam(a_name) );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetParam)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_STRING_PARAM( a_name, 0);
		GM_CHECK_FLOAT_OR_INT_PARAM( val, 1);
		GM_GET_THIS_PTR(FmodEventInst, ptr);

		a_thread->PushBool( ptr->SetParam( a_name, val ) );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetParamLerp)
	{
		GM_CHECK_NUM_PARAMS(3);
		GM_CHECK_STRING_PARAM( a_name, 0);
		GM_CHECK_FLOAT_OR_INT_PARAM( a_goal_val, 1);
		GM_CHECK_FLOAT_PARAM( a_lerp_coeff, 2);
		GM_GET_THIS_PTR(FmodEventInst, ptr);

		a_thread->PushBool( ptr->SetParamLerp( a_name, a_goal_val, a_lerp_coeff ) );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(Stop)
	{
		GM_INT_PARAM( a_fadeout, 0, 0 );
		GM_GET_THIS_PTR(FmodEventInst, ptr);
		ptr->Stop( a_fadeout != 0 ? true:false );
		return GM_OK;
	}

	GM_GEN_MEMFUNC_VOID_VOID( FmodEventInst, Start )
	GM_GEN_MEMFUNC_V3_VOID( FmodEventInst, GetPos )
	GM_GEN_MEMFUNC_V3_VOID( FmodEventInst, GetVel )
	GM_GEN_MEMFUNC_V3_VOID( FmodEventInst, GetOrientationForward )
	GM_GEN_MEMFUNC_V3_VOID( FmodEventInst, GetOrientationUp )
	GM_GEN_MEMFUNC_VOID_V3( FmodEventInst, SetPos )
	GM_GEN_MEMFUNC_VOID_V3( FmodEventInst, SetVel )
	GM_GEN_MEMFUNC_VOID_V3_V3( FmodEventInst, SetOrientation )
	GM_GEN_MEMFUNC_INT_VOID( FmodEventInst, GetPlaybackState )
	GM_GEN_MEMFUNC_INT_VOID( FmodEventInst, GetLoadingState )
	GM_GEN_MEMFUNC_INT_VOID( FmodEventInst, GetTimelinePos )
	GM_GEN_MEMFUNC_VOID_INT( FmodEventInst, SetTimelinePos )
}

GM_REG_MEM_BEGIN(FmodEventInst)
GM_REG_MEMFUNC( FmodEventInst, CreateSubEvent )
GM_REG_MEMFUNC( FmodEventInst, Start )
GM_REG_MEMFUNC( FmodEventInst, Stop )
GM_REG_MEMFUNC( FmodEventInst, GetParam )
GM_REG_MEMFUNC( FmodEventInst, SetParam )
GM_REG_MEMFUNC( FmodEventInst, SetParamLerp )
GM_REG_MEMFUNC( FmodEventInst, GetTimelinePos )
GM_REG_MEMFUNC( FmodEventInst, SetTimelinePos )
GM_REG_MEMFUNC( FmodEventInst, GetPos )
GM_REG_MEMFUNC( FmodEventInst, GetVel )
GM_REG_MEMFUNC( FmodEventInst, GetOrientationForward )
GM_REG_MEMFUNC( FmodEventInst, GetOrientationUp )
GM_REG_MEMFUNC( FmodEventInst, SetPos )
GM_REG_MEMFUNC( FmodEventInst, SetVel )
GM_REG_MEMFUNC( FmodEventInst, SetOrientation )
GM_REG_MEMFUNC( FmodEventInst, GetPlaybackState )
GM_REG_MEMFUNC( FmodEventInst, GetLoadingState )
GM_REG_HANDLED_DESTRUCTORS(FmodEventInst)
GM_REG_MEM_END()
GM_BIND_DEFINE(FmodEventInst)

}

#endif // FMOD_ENABLED