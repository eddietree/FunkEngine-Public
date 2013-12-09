#include "SoundChannel.h"

#include "SoundManager.h"

#include <gm/gmBind.h>
#include <common/Debug.h>
#include <audio/ALIncludes.h>
#include <math/Constants.h>

namespace funk
{

v3 GetV3( unsigned int a_sid, int a_param )
{
	float vals[3];
	alGetSourcefv( a_sid, a_param, vals );
	return v3(vals[0], vals[1], vals[2]);
}

float GetFloat( unsigned int a_sid, int a_param )
{
	float val = 0.0f;
	alGetSourcef( a_sid, a_param, &val );
	return val;
}

SoundChannel::SoundChannel() : m_id(0)
{}

SoundChannel::~SoundChannel()
{
	Release();
}

void SoundChannel::Init()
{
	// must not already been initialized
	ASSERT( m_id == 0 );

	// source id
	alGenSources( 1, &m_id );
	SoundManager::Ref().CheckError();
	ASSERT( m_id != 0 );
	ASSERT( alIsSource(m_id) );
}

void SoundChannel::Reset()
{
	ASSERT( m_id != 0 );

	// remove all buffers
	UnbindBuffer();

	// must set manually beacuse of radToDeg floating pt imprecision
	alSource3f(m_id, AL_DIRECTION, 0.0f, 0.0f, 0.0f );
	alSourcef(m_id, AL_CONE_INNER_ANGLE, 360.0f );
	alSourcef(m_id, AL_CONE_OUTER_ANGLE, 360.0f );
	alSourcef(m_id, AL_CONE_OUTER_GAIN, 0.0f );

	SetPitch(1.0f);
	SetVolume(1.0f);
	SetPos(v3(0.0f));
	SetVel(v3(0.0f));
	SetRelativeToListener(false);
	SetLooping(false);
}

bool SoundChannel::IsActive() const
{
	SoundChannelState state = GetState();
	return state == SOUNDSOURCE_PLAYING || state == SOUNDSOURCE_PAUSED;
}

SoundChannelState SoundChannel::GetState() const
{
	int state = 0;
	alGetSourcei( m_id, AL_SOURCE_STATE, &state );

	switch( state )
	{
		case AL_INITIAL	: return SOUNDSOURCE_INITIAL;
		case AL_PLAYING	: return SOUNDSOURCE_PLAYING;
		case AL_STOPPED	: return SOUNDSOURCE_STOPPED;
		case AL_PAUSED	: return SOUNDSOURCE_PAUSED;
	}

	// should not get here
	ERROR_MSG("Unrecognized sound source state");
	return SOUNDSOURCE_NULL;
}

void SoundChannel::BindBuffer( StrongHandle<SoundBuffer> a_buffer )
{
	ASSERT( a_buffer != nullptr );

	m_buffer = a_buffer;
	alSourcei( m_id, AL_BUFFER, m_buffer->GetId() );
	SoundManager::Ref().CheckError();
}

void SoundChannel::UnbindBuffer()
{
	Stop();
	m_buffer = nullptr;
	alSourcei( m_id, AL_BUFFER, AL_NONE );
	SoundManager::Ref().CheckError();
}

void SoundChannel::Release()
{
	UnbindBuffer();

	if ( m_id )
	{
		alDeleteSources(1, &m_id );
		m_id = 0;
	}

	SoundManager::Ref().CheckError();
}

void SoundChannel::Play()
{
	// calling this function on AL_PLAYING state will rewind
	alSourcePlay(m_id);
	SoundManager::Ref().CheckError();
}

void SoundChannel::Stop()
{
	alSourceStop(m_id);
}

void SoundChannel::SetVolume( float vol )
{
	alSourcef(m_id, AL_GAIN, vol);
}

float SoundChannel::GetVolume() const
{
	return GetFloat( m_id, AL_GAIN );
}

float SoundChannel::GetPitch() const
{
	return GetFloat( m_id, AL_PITCH );
}

void SoundChannel::SetPitch(float a_pitch)
{
	alSourcef(m_id, AL_PITCH, a_pitch);
}

float SoundChannel::GetCurrPosSeconds() const
{
	return GetFloat( m_id, AL_SEC_OFFSET );
}

void SoundChannel::SetPosSeconds( float a_seconds )
{
	alSourcef( m_id, AL_SEC_OFFSET, a_seconds );
}

v3 SoundChannel::GetPos() const
{
	return GetV3( m_id, AL_POSITION );
}

void SoundChannel::SetPos( v3 a_pos )
{
	alSource3f( m_id, AL_POSITION, a_pos.x, a_pos.y, a_pos.z );
}

v3 SoundChannel::GetVel() const
{
	return GetV3( m_id, AL_VELOCITY );
}

void SoundChannel::SetVel( v3 a_vel )
{
	alSource3f( m_id, AL_VELOCITY, a_vel.x, a_vel.y, a_vel.z );
}

v3 SoundChannel::GetDirection() const
{
	return GetV3( m_id, AL_DIRECTION );
}

void SoundChannel::SetDirection( v3 a_dir )
{
	alSource3f( m_id, AL_DIRECTION, a_dir.x, a_dir.y, a_dir.z );
}

float SoundChannel::GetConeInnerAngle()
{
	return GetFloat( m_id, AL_CONE_INNER_ANGLE ) * PI / 180.0f;
}

void SoundChannel::SetConeInnerAngle( float a_radians )
{
	alSourcef( m_id, AL_CONE_INNER_ANGLE, a_radians * 180.0f / PI );
}

float SoundChannel::GetConeOuterAngle()
{
	return GetFloat( m_id, AL_CONE_OUTER_ANGLE ) * PI / 180.0f;
}

void SoundChannel::SetConeOuterGain( float a_gain )
{
	alSourcef( m_id, AL_CONE_OUTER_GAIN, a_gain );
}

float SoundChannel::GetConeOuterGain()
{
	return GetFloat( m_id, AL_CONE_OUTER_GAIN );
}

void SoundChannel::SetConeOuterAngle( float a_radians )	
{
	alSourcef( m_id, AL_CONE_OUTER_ANGLE, a_radians * 180.0f / PI );
}

void SoundChannel::SetRelativeToListener( bool a_relative )
{
	alSourcei( m_id, AL_SOURCE_RELATIVE, a_relative ? AL_TRUE : AL_FALSE );
}

bool SoundChannel::GetRelativeToListener() const
{
	int relative = 0;
	alGetSourcei( m_id, AL_SOURCE_RELATIVE, &relative );
	return relative != 0;
}

float SoundChannel::GetPan() const
{
	v3 pos = GetPos();
	return pos.x;
}

void SoundChannel::SetPan(float pan)
{
	SetRelativeToListener(true);
	SetPos( v3(pan, 0.0f,0.3f) );
}

bool SoundChannel::IsPlaying() const
{
	return GetState() == SOUNDSOURCE_PLAYING;
}

void SoundChannel::SetPause( bool pause )
{
	if ( IsActive() )
	{
		alSourcePause( m_id );
	}
}

bool SoundChannel::IsLooping() const
{
	int looping = 0;
	alGetSourcei( m_id, AL_LOOPING, &looping );
	return looping == AL_TRUE;
}

void SoundChannel::SetLooping( bool a_loop )
{
	alSourcei( m_id, AL_LOOPING, a_loop ? AL_TRUE : AL_FALSE );
}

GM_REG_NAMESPACE(SoundChannel)
{
	GM_MEMFUNC_CONSTRUCTOR(SoundChannel)
	{
		GM_CHECK_NUM_PARAMS(0);

		StrongHandle<SoundChannel> p = new SoundChannel();
		p->Init();
		GM_PUSH_USER_HANDLED( SoundChannel, p.Get() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(BindBuffer)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_USER_PARAM_PTR( SoundBuffer, buffer, 0 );
		GM_GET_THIS_PTR(SoundChannel, ptr);

		ptr->BindBuffer(buffer->GetHandle());
		return GM_OK;
	}

	GM_GEN_MEMFUNC_BOOL_VOID( SoundChannel, IsActive )
	GM_GEN_MEMFUNC_VOID_BOOL( SoundChannel, SetPause )
	GM_GEN_MEMFUNC_VOID_VOID( SoundChannel, Play )
	GM_GEN_MEMFUNC_FLOAT_VOID( SoundChannel, GetVolume )
	GM_GEN_MEMFUNC_VOID_FLOAT( SoundChannel, SetVolume )
	GM_GEN_MEMFUNC_FLOAT_VOID( SoundChannel, GetPitch )
	GM_GEN_MEMFUNC_FLOAT_VOID( SoundChannel, GetPan )
	GM_GEN_MEMFUNC_VOID_FLOAT( SoundChannel, SetPitch )
	GM_GEN_MEMFUNC_VOID_FLOAT( SoundChannel, SetPan )
	GM_GEN_MEMFUNC_INT_VOID( SoundChannel, IsPlaying )
	GM_GEN_MEMFUNC_VOID_VOID( SoundChannel, Stop )
	GM_GEN_MEMFUNC_BOOL_VOID( SoundChannel, IsLooping )
	GM_GEN_MEMFUNC_VOID_BOOL( SoundChannel, SetLooping )
	GM_GEN_MEMFUNC_FLOAT_VOID( SoundChannel, GetCurrPosSeconds )
	GM_GEN_MEMFUNC_VOID_FLOAT( SoundChannel, SetPosSeconds )
	GM_GEN_MEMFUNC_VOID_VOID( SoundChannel, UnbindBuffer )

	GM_GEN_MEMFUNC_VOID_V3( SoundChannel, SetPos )
	GM_GEN_MEMFUNC_V3_VOID( SoundChannel, GetPos )
	GM_GEN_MEMFUNC_VOID_V3( SoundChannel, SetVel )
	GM_GEN_MEMFUNC_V3_VOID( SoundChannel, GetVel )

	GM_GEN_MEMFUNC_V3_VOID( SoundChannel, GetDirection )
	GM_GEN_MEMFUNC_VOID_V3( SoundChannel, SetDirection )
	GM_GEN_MEMFUNC_FLOAT_VOID( SoundChannel, GetConeInnerAngle )
	GM_GEN_MEMFUNC_VOID_FLOAT( SoundChannel, SetConeInnerAngle )
	GM_GEN_MEMFUNC_FLOAT_VOID( SoundChannel, GetConeOuterAngle )
	GM_GEN_MEMFUNC_VOID_FLOAT( SoundChannel, SetConeOuterAngle )
	GM_GEN_MEMFUNC_FLOAT_VOID( SoundChannel, GetConeOuterGain )
	GM_GEN_MEMFUNC_VOID_FLOAT( SoundChannel, SetConeOuterGain )
}

GM_REG_MEM_BEGIN(SoundChannel)
GM_REG_MEMFUNC( SoundChannel, IsActive )
GM_REG_MEMFUNC( SoundChannel, BindBuffer )
GM_REG_MEMFUNC( SoundChannel, UnbindBuffer )
GM_REG_MEMFUNC( SoundChannel, Play )
GM_REG_MEMFUNC( SoundChannel, Stop )
GM_REG_MEMFUNC( SoundChannel, IsLooping )
GM_REG_MEMFUNC( SoundChannel, SetLooping )
GM_REG_MEMFUNC( SoundChannel, GetVolume )
GM_REG_MEMFUNC( SoundChannel, SetVolume )
GM_REG_MEMFUNC( SoundChannel, GetPitch )
GM_REG_MEMFUNC( SoundChannel, SetPitch )
GM_REG_MEMFUNC( SoundChannel, SetPause )
GM_REG_MEMFUNC( SoundChannel, GetPan )
GM_REG_MEMFUNC( SoundChannel, SetPan )
GM_REG_MEMFUNC( SoundChannel, GetCurrPosSeconds )
GM_REG_MEMFUNC( SoundChannel, SetPosSeconds )
GM_REG_MEMFUNC( SoundChannel, IsPlaying )
GM_REG_MEMFUNC( SoundChannel, SetPos )
GM_REG_MEMFUNC( SoundChannel, GetPos )
GM_REG_MEMFUNC( SoundChannel, SetVel )
GM_REG_MEMFUNC( SoundChannel, GetVel )
GM_REG_MEMFUNC( SoundChannel, GetDirection )
GM_REG_MEMFUNC( SoundChannel, SetDirection )
GM_REG_MEMFUNC( SoundChannel, GetConeInnerAngle )
GM_REG_MEMFUNC( SoundChannel, SetConeInnerAngle )
GM_REG_MEMFUNC( SoundChannel, GetConeOuterAngle )
GM_REG_MEMFUNC( SoundChannel, SetConeOuterAngle )
GM_REG_MEMFUNC( SoundChannel, GetConeOuterGain )
GM_REG_MEMFUNC( SoundChannel, SetConeOuterGain )
GM_REG_HANDLED_DESTRUCTORS(SoundChannel)
GM_REG_MEM_END()
GM_BIND_DEFINE(SoundChannel)

}