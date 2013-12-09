#pragma once

#include <common/Singleton.h>

#include <fstream>

namespace funk
{
    class Log : public Singleton<Log>
	{
	private:

		std::ofstream m_file;
		
    public:

		void AddEntry(const char* text, bool newLine = false );
		void AddEntryFormat( const char* a_fmt, ...);

    private:
        friend Singleton<Log>;
		Log();
		~Log();
    };
}
