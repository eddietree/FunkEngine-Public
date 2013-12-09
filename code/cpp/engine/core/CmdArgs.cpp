#include "CmdArgs.h"

#include <cstring>
#include <stdlib.h>

namespace funk
{	
	CmdArgs::CmdArgs( int a_argc, char* a_argv[] )
	{	
		const char* key = nullptr;
		const char* search_result = nullptr;

		for ( int i = 1; i < a_argc; ++i )
		{
			char* arg = a_argv[i];
			//TRACE("arg %d = '%s'\n", i, arg );

			// for flag types (ex: "--fullscreen")
			const char* flag_prefix = "--";
			search_result =  strstr( arg, flag_prefix );
			if ( search_result != nullptr && search_result == arg )
			{
				m_flags.push_back( arg+2 );
				continue;
			}

			// key is set..
			if ( key != nullptr )
			{
				ArgData arg_data = { key, arg };
				m_args.push_back(arg_data);
				key = nullptr;
				continue;
			}

			// for arg types (ex: "-w")
			const char* arg_prefix = "-";
			search_result =  strstr( arg, arg_prefix );
			if ( search_result != nullptr && search_result == arg )
			{
				key = arg+1;
				continue;
			}
		}
	}

	bool CmdArgs::IsFlagSet( const char* a_flag_name ) const
	{
		for( size_t i = 0; i < m_flags.size(); ++i )
		{
			if ( strcmp( a_flag_name, m_flags[i].c_str() ) == 0 )
				return true;
		}

		return false;
	}
	
	int CmdArgs::GetArgValueInt( const char * a_arg, int a_default_value ) const
	{
		const char* arg = GetArg( a_arg );
		if ( arg == nullptr ) return a_default_value;

		return atoi(arg);
	}

	float CmdArgs::GetArgValueFloat( const char * a_arg, float a_default_value ) const
	{
		const char* arg = GetArg( a_arg );
		if ( arg == nullptr ) return a_default_value;

		return (float)atof(arg);
	}

	const char* CmdArgs::GetArgValueStr( const char * a_arg ) const
	{
		const char* arg = GetArg( a_arg );
		return arg;
	}

	const char* CmdArgs::GetArg( const char* a_arg ) const
	{
		for( size_t i = 0; i < m_args.size(); ++i )
		{
			if ( strcmp( a_arg, m_args[i].key.c_str() ) == 0 )
				return m_args[i].value.c_str();
		}

		return nullptr;
	}

}