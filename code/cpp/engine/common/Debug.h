#pragma once

#if !defined(FUNK_FINAL)

    #include <iostream>
	#include <assert.h>

	// define this if you want to trace to visaul studio output window
	//#define TRACE_TO_VISUALSTUDIO_OUTPUT_WINDOW

	#define ASSERT_MECHANISM_INTERRUPT_X86() __asm { int 3 }
	#define ASSERT_MECHANISM_INTERRUPT_INTRINSIC() __debugbreak();

    #if defined(_WIN32)
        #define ASSERT_MECHANISM_ASSERT ASSERT_MECHANISM_INTERRUPT_X86
    #else
        #define ASSERT_MECHANISM_ASSERT() assert(false)
    #endif


	#if defined(_WIN32) && defined(TRACE_TO_VISUALSTUDIO_OUTPUT_WINDOW)
		void TRACE( const char * format, ... );	
	#else
		#define TRACE(format, ... ) printf( format, ##__VA_ARGS__ )
	#endif // _WIN32

	#define ERROR_MSG(format, ... )				{ TRACE( "[ERROR] " format "\n", ##__VA_ARGS__ ); ASSERT_MECHANISM_ASSERT();}
	#define ASSERT(cond)						if (!(cond)) { TRACE( "[ERROR] Assertion failed: '%s', on file %s:%d\n", #cond, __FILE__, __LINE__ ); ASSERT_MECHANISM_ASSERT(); }
	#define CHECK(cond, format, ...)			if (!(cond)) { TRACE( "[ERROR] Assertion failed: '%s', on file %s:%d -  " format "\n", #cond, __FILE__, __LINE__, ##__VA_ARGS__ ); ASSERT_MECHANISM_ASSERT(); }
	#define WARN(cond,format, ...)				{ if (!cond) {TRACE( "[WARNING] " format "\n", ##__VA_ARGS__ );} }

#else // FUNK_FINAL

	#define TRACE(format, ... )
	#define ERROR_MSG(format, ... ) LOG(format, ##__VA_ARGS__ )
	#define ASSERT(cond)
	#define CHECK(cond, format, ...)
	#define WARN(cond,format, ...)

#endif // FUNK_FINAL

// logging stuff
void LOG_SAMELINE( const char* format, ...);
#define LOG(format, ...)					LOG_SAMELINE(format"\n", ##__VA_ARGS__);
#define LOG_ERROR(format, ...)				LOG_SAMELINE("[ERROR] " format "\n", ##__VA_ARGS__);

#define HALT_ERROR() exit(1)
void MESSAGE_BOX( const char * title, const char * format, ... );
