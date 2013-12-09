#include "Debug.h"
#include <common/Log.h>

#include <stdarg.h>

#if defined(_WIN32) && defined(TRACE_TO_VISUALSTUDIO_OUTPUT_WINDOW)

	#define WIN32_LEAN_AND_MEAN
	#define VC_EXTRALEAN

	#include <Windows.h>

	void TRACE( const char * format, ... ) 
	{
		char buffer[1024];

		va_list arg;
		va_start(arg, format);
		_vsnprintf(buffer, sizeof(buffer), format, arg);
		va_end(arg);

        buffer[1023] = '\0';

		OutputDebugString(buffer);
	}

	void MESSAGE_BOX( const char * title, const char * format, ... )
	{
		char buffer[1024];
		va_list arg;
		va_start(arg, format);
		_vsnprintf(buffer, sizeof(buffer), format, arg);
		va_end(arg);

        buffer[1023] = '\0';

		MessageBoxA( nullptr, buffer, title, MB_OK );
	}
#else
	void MESSAGE_BOX( const char * title, const char * format, ... )
	{
        char buffer[1024];
		va_list arg;
		va_start(arg, format);
		vsnprintf(buffer, sizeof(buffer), format, arg);
		va_end(arg);
        
		TRACE( buffer );
	}

#endif // #if defined(_WIN32) && defined(TRACE_TO_VISUALSTUDIO_OUTPUT_WINDOW)

void LOG_SAMELINE( const char * format, ... ) 
{
	char buffer[1024];

    va_list args;
    va_start(args, format);
	vsprintf( buffer, format, args );
    va_end(args);

	TRACE("%s", buffer );

	if ( funk::Log::InstExists() )
		funk::Log::Ref().AddEntry( buffer );
}
