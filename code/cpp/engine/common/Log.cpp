#include "Log.h"

#include <system/Config.h>
#include <common/Debug.h>
#include <stdarg.h>

namespace funk
{
	const char * LOG_FILENAME = "LOG.txt";

	Log::Log() 
	{
        #if !PLATFORM_MOBILE
		m_file.open(LOG_FILENAME);
		CHECK( m_file.is_open(), "Cannot open log file %s", LOG_FILENAME );
        #endif
	}
	
	Log::~Log()
	{
		AddEntry("Closing Log...");
		m_file.close();
	}

	void Log::AddEntry(const char* text, bool newLine)
	{
		if ( !text ) return;

		#if !PLATFORM_MOBILE
		// log to file
		if ( m_file.is_open() ) 
		{
			m_file << text;
			if ( newLine ) m_file << std::endl;
			m_file << std::flush;
			ASSERT(!m_file.bad());
		}
		#endif	
	}

	void Log::AddEntryFormat( const char* a_fmt, ...)
	{
		char buffer[1024];

		va_list args;
		va_start(args, a_fmt);
		vsprintf( buffer, a_fmt, args );
		va_end(args);

		AddEntry(buffer);
	}

}