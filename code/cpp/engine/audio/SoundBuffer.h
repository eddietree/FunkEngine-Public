#pragma once

#include <common/HandledObj.h>
#include <gm/gmBindHeader.h>

namespace funk
{
	class SoundBuffer : public HandledObj<SoundBuffer>
	{
	private:
		
		unsigned int m_id; // from AL
		
		float m_buffer_duration_seconds;

		// buffer properties
		int m_length_in_samples;
		int m_freq;
		int m_channels;
		int m_num_bytes;
		int m_bits;

	public:

		SoundBuffer();
		~SoundBuffer();		
		void Init( const char * a_file_name );

		// get params
		unsigned int GetId() const { return m_id; }
		float GetDurationSecs() const { return m_buffer_duration_seconds; }
		int GetFreq() const { return m_freq; }
		int GetNumChannels() const { return m_channels; }
		int GetNumBytes() const { return m_num_bytes; }

		GM_BIND_TYPEID(SoundBuffer);

	private:

		void InitWave( const char * a_filename );
		void InitOgg( const char * a_filename );
		void Release();

		void GetBufferProperties();
	};

	GM_BIND_DECL( SoundBuffer );
}