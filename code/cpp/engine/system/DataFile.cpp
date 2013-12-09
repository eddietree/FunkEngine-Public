#include "DataFile.h"

#include <string.h>

#include "FileUtil.h"
#include <common/Debug.h>
#include <system/Config.h>

namespace funk
{
	DataFile::DataFile() : m_data( nullptr ), m_num_bytes(0)
	{
	}

	DataFile::~DataFile()
	{
		Release();
	}

	void DataFile::Load( const char* a_filename, bool binary )
	{
		// free old data
		if ( m_data ) Release();

		m_data = (char*)ReadFile( a_filename, &m_num_bytes, binary );
		ASSERT(m_data);

		// grab file extension
		const char * ext = GetFileExt(a_filename);
		size_t ext_len = strlen(ext);
		ASSERT( ext_len < sizeof(m_ext) );
		memcpy( m_ext, ext, ext_len );
		m_ext[ext_len] = '\0';
	}

	void DataFile::Release()
	{
		// apple reads data directly from file bundle (does not copy bytes to buffers)
		#ifndef __APPLE__
		delete [] m_data;
		#endif

		m_num_bytes = 0;
		m_data = nullptr;
	}
}
