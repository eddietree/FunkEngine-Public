
#pragma once

#include <string>
#include <vector>

namespace funk
{
	class CmdArgs
	{
	private:

		struct ArgData
		{
			std::string key;
			std::string value;
		};

		std::vector<ArgData> m_args;
		std::vector<std::string> m_flags;

	public:
		CmdArgs( int a_argc, char* a_argv[] );

		// check flags ("--fullscreen")
		bool IsFlagSet( const char* a_flag_name ) const;

		// get values
		int			GetArgValueInt( const char * a_arg, int a_default_value = 0 ) const;
		float		GetArgValueFloat( const char * a_arg, float a_default_value = 0 ) const;
		const char* GetArgValueStr( const char * a_arg ) const;
	
	private:

		const char* GetArg( const char* a_arg ) const;
	};
}
