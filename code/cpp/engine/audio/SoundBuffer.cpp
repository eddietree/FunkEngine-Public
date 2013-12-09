#include "SoundBuffer.h"

#include "SoundManager.h"

#include <gm/gmBind.h>
#include <common/Debug.h>
#include <audio/ALIncludes.h>
#include <system/Config.h>
#include <system/FileUtil.h>
#include <system/DataFile.h>


#include <stdint.h>
#include <memory>
#include <vector>

#if !PLATFORM_MOBILE
#define USE_VORBIS
#endif

#if defined(USE_VORBIS)
#include <vorbis/vorbisfile.h>
#endif

namespace funk
{
SoundBuffer::SoundBuffer() 
	:	m_id(0), 
		m_buffer_duration_seconds(0),
		m_length_in_samples(0),
		m_freq(0),
		m_channels(0),
		m_num_bytes(0),
		m_bits(0)
{
}

SoundBuffer::~SoundBuffer()
{
	Release();
}

void SoundBuffer::Init( const char * a_file_name )
{
	ASSERT(a_file_name);

	// buffer id
	alGenBuffers( 1, &m_id );
	ASSERT( m_id != 0 );
	ASSERT( alIsBuffer(m_id) );
	SoundManager::Ref().CheckError();

	// load file based on extension
	const char * ext = GetFileExt( a_file_name );
	if ( strcmp( ext, "wav" ) == 0 )
		InitWave(a_file_name);
	else if ( strcmp( ext, "ogg" ) == 0 )
		InitOgg(a_file_name);
	else
		ERROR_MSG("Cannot load sound '%s': unrecognized file extension '%s'", a_file_name, ext );

	// TEMPFIX: basically, if we turn off vorbis, GetProperties will crash..
	#if defined(USE_VORBIS)
	GetBufferProperties();
	#endif

	SoundManager::Ref().CheckError();
}

void SoundBuffer::InitWave( const char * a_filename )
{
	// load file to memory
	DataFile data_file;
    data_file.Load( a_filename, true );

	// based on: http://www.dunsanyinteractive.com/blogs/oliver/?p=72

	struct RIFF_Header 
	{
		char	chunkID[4];
		int32_t chunkSize;//size not including chunkSize or chunkID
		char	format[4];
	};

	struct WAVE_Format 
	{
		char subChunkID[4];
		int32_t subChunkSize;
		int16_t audioFormat;
		int16_t numChannels;
		int32_t sampleRate;
		int32_t byteRate;
		int16_t blockAlign;
		int16_t bitsPerSample;
	};

	struct WAVE_Data 
	{
		char	subChunkID[4]; //should contain the word data
		int32_t subChunk2Size; //Stores the size of the data block
	};

	struct WAVE_Header
	{
		RIFF_Header riff_header;
		WAVE_Format wave_format;
		WAVE_Data	wave_data;
	};

	WAVE_Header header;
	memcpy( &header, data_file.GetData(), sizeof(header) );

	// check integrity of header
	ASSERT( strncmp( header.riff_header.chunkID, "RIFF", 4 ) == 0 );
	ASSERT( strncmp( header.riff_header.format, "WAVE", 4 ) == 0 );
	ASSERT( strncmp( header.wave_format.subChunkID, "fmt ", 4 ) == 0 );
	ASSERT( strncmp( header.wave_data.subChunkID, "data", 4 ) == 0 );

	char* data = (char*)data_file.GetData() + sizeof(header);

	// determine the wave data format
	ALenum format;
	if ( header.wave_format.numChannels == 1 ) // single channel
	{
		if ( header.wave_format.bitsPerSample == 8 )
			format = AL_FORMAT_MONO8;
		else if ( header.wave_format.bitsPerSample == 16 )
			format = AL_FORMAT_MONO16;
		else
			ERROR_MSG("Unrecognized bitrate: %d", header.wave_format.bitsPerSample );
	} 
	else if ( header.wave_format.numChannels == 2)  // double channel
	{
		if ( header.wave_format.bitsPerSample == 8 )
			format = AL_FORMAT_STEREO8;
		else if ( header.wave_format.bitsPerSample == 16 )
			format = AL_FORMAT_STEREO16;
		else
			ERROR_MSG("Unrecognized bitrate: %d", header.wave_format.bitsPerSample );
	}
	else
	{
		ERROR_MSG("Unrecognized number of channels: %d", header.wave_format.numChannels );
	}

	ALsizei size = header.wave_data.subChunk2Size;
	ALsizei freq = header.wave_format.sampleRate;

	// output the data
	ASSERT( alIsBuffer(m_id) );
	alBufferData( m_id, format, data, size, freq);
}

void SoundBuffer::InitOgg( const char * a_filename )
{
#ifdef USE_VORBIS
	// http://www.gamedev.net/page/resources/_/technical/game-programming/introduction-to-ogg-vorbis-r2031
	OggVorbis_File ogg_file;

	// load ogg
    const char * full_path = GetFullPath( a_filename );
	int err_val = ov_fopen( full_path, &ogg_file );
	//int err_val = ov_open_callbacks( nullptr, &ogg_file, (const char*)a_data_file.GetData(), a_data_file.GetNumBytes(), OV_CALLBACKS_STREAMONLY );
	CHECK( err_val == 0, "OGG File load error %d", err_val );
	vorbis_info * ogg_info = ov_info(&ogg_file, -1);
	ASSERT( ogg_info );

	// ogg format
	ALenum al_format;
	if ( ogg_info->channels == 1 ) 
		al_format = AL_FORMAT_MONO16;
	else if ( ogg_info->channels == 2 )
		al_format = AL_FORMAT_STEREO16;
	else
		ERROR_MSG("OGG - Unrecognized num channels : %d", ogg_info->channels );

	// num samples
	ogg_int64_t total_num_pcm_samples = ov_pcm_total( &ogg_file, -1 );
	ASSERT( total_num_pcm_samples > 0 );

	// reserve space for the buffer
	std::vector<char> buffer;
	buffer.reserve( (size_t)(total_num_pcm_samples * 2 * ogg_info->channels) );

	char buff_array[32768];    // Local fixed size array
	int bit_stream = 0;
	int num_bytes_read = 0;

	// read all the data
	do 
	{
		num_bytes_read = ov_read(&ogg_file, buff_array, sizeof(buff_array), 0, 2, 1, &bit_stream);
		buffer.insert(buffer.end(), buff_array, buff_array + num_bytes_read);
	} while (num_bytes_read > 0);
	 
	// check size size
	ASSERT( total_num_pcm_samples * 2 * ogg_info->channels  == buffer.size() );

	// output the data
	ALsizei al_size = buffer.size();
	ALsizei al_freq = ogg_info->rate;
	ASSERT( alIsBuffer(m_id) );
	alBufferData( m_id, al_format, &buffer[0], al_size, al_freq);
	
	// close the file
	ov_clear(&ogg_file);
#endif
}

void SoundBuffer::GetBufferProperties()
{
	ASSERT( m_id != 0 );

	alGetBufferi(m_id, AL_SIZE, &m_num_bytes);
	alGetBufferi(m_id, AL_CHANNELS, &m_channels);
	alGetBufferi(m_id, AL_BITS, &m_bits);
	alGetBufferi(m_id, AL_FREQUENCY, &m_freq);

	m_length_in_samples = m_num_bytes * 8 / (m_channels * m_bits);
	m_buffer_duration_seconds = (float)m_length_in_samples / (float)m_freq;
}

void SoundBuffer::Release()
{
	if ( m_id )
	{
		alDeleteBuffers(1, &m_id);
		m_id = 0;
	}
}

GM_REG_NAMESPACE(SoundBuffer)
{
	GM_MEMFUNC_CONSTRUCTOR(SoundBuffer)
	{
		GM_CHECK_NUM_PARAMS(1);

		GM_CHECK_STRING_PARAM( file, 0 );

		StrongHandle<SoundBuffer> p = new SoundBuffer;
		p->Init(file);

		GM_PUSH_USER_HANDLED( SoundBuffer, p.Get() );
		return GM_OK;
	}

	GM_GEN_MEMFUNC_FLOAT_VOID( SoundBuffer, GetDurationSecs )
	GM_GEN_MEMFUNC_INT_VOID( SoundBuffer, GetFreq )
	GM_GEN_MEMFUNC_INT_VOID( SoundBuffer, GetNumChannels )
	GM_GEN_MEMFUNC_INT_VOID( SoundBuffer, GetNumBytes )
	GM_GEN_MEMFUNC_INT_VOID( SoundBuffer, GetId )
}

GM_REG_MEM_BEGIN(SoundBuffer)
GM_REG_MEMFUNC( SoundBuffer, GetDurationSecs )
GM_REG_MEMFUNC( SoundBuffer, GetFreq )
GM_REG_MEMFUNC( SoundBuffer, GetNumChannels )
GM_REG_MEMFUNC( SoundBuffer, GetNumBytes )
GM_REG_MEMFUNC( SoundBuffer, GetId )
GM_REG_HANDLED_DESTRUCTORS(SoundBuffer)
GM_REG_MEM_END()
GM_BIND_DEFINE(SoundBuffer)

}