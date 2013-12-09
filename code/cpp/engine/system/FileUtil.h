#pragma once

#include <string.h>

namespace funk
{
	const char* ReadFile( const char* a_filename, int * out_num_bytes = nullptr, bool a_binary = false );
	bool FileExists( const char* a_filename );
    	
	inline const char* GetFileExt( const char* a_filename )
	{
		const char * ext = strrchr( a_filename, '.' ) + 1;
		return ext;
	}

    const char * GetFullPath( const char * a_filename );
}
