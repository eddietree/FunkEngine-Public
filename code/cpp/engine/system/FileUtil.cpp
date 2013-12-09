#include "FileUtil.h"

#if defined(_WIN32)

#include <common/Debug.h>
#include <fstream>

namespace funk
{
	const char * ReadFile( const char* a_filename, int * out_num_bytes, bool a_binary )
	{
		std::ifstream file( a_filename, std::ios::binary );

		if (  !file.is_open() )
		{ 
			CHECK( false, "Cannot open file '%s'\n", a_filename );
			return NULL;
		}

		file.seekg( 0, std::ios::end ); 
		size_t size = (size_t)file.tellg();
		file.seekg( 0, std::ios::beg );

		char* data = new char[ size + 1 ];
		file.read( data, size );
		file.close();

		data[size] = '\0';

		if ( out_num_bytes ) *out_num_bytes = size;

		return data;
	}

	bool FileExists( const char* a_filename )
	{
		ASSERT(a_filename);
		std::ifstream ifile(a_filename);
		return ifile ? true : false;
	}
    
    const char * GetFullPath( const char * a_filename )
    {
        return a_filename;
    }
}

#endif // defined(_WIN32)