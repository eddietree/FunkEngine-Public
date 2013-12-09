#include "gmUtilEx.h"

#include <time.h>
#include <fstream>
#include <chrono>
#include <thread>

#include <system/DataFile.h>
#include <system/FileUtil.h>
#include <system/FileModificationChecker.h>
#include <common/Debug.h>
#include <vm/VirtualMachine.h>

#include "gmMachine.h"
#include "gmTableObject.h"
#include "gmStreamBuffer.h"

#include <math/v2.h>
#include <math/v3.h>

// waits for file to be modified before attempting a recompile
#if defined(_WIN32)
#define GM_COMPILE_ERROR_WAIT_FILETOUCH
#endif

using namespace funk;

#define GM_RESOURCE_GLOBAL_KEY "g_fileData"

bool gmFileExists( const char * file )
{
	return FileExists( file );
}

int gmCompileStr( gmMachine *vm, const char* file )
{
	// If using bytecode
	if ( VirtualMachine::Get()->IsUsingByteCode() )
	{
		int threadId;

		DataFile file_data;
        file_data.Load(file, true);
		const char* code = (const char*)file_data.GetData();

		gmStreamBufferStatic readBuffer;
		readBuffer.Open(code, file_data.GetNumBytes() );
		if ( !vm->ExecuteLib(readBuffer, &threadId, true ) )
		{
			ERROR_MSG("Gamemonkey failed to run bytecode of '%s'", file);
		}
		
		return threadId;
	}

	// Not using bytecode, compiling
	while(true)
	{
		int threadId;
		DataFile file_data;
        file_data.Load(file);
		const char* code = (const char*)file_data.GetData();

		// failed
		if ( !code ) return 0;

		if ( vm->GetDebugMode() ) vm->AddSourceCode( code, file );
		int err = vm->CheckSyntax( code );
		
		if ( !err ) 
		{
			TRACE("Compiled '%s'\n", file );
			vm->ExecuteString(code, &threadId);
			
			return threadId;
		}

		gmLog & compileLog = vm->GetLog();
		bool first = true;
		const char *msg = compileLog.GetEntry(first);
		if(msg)	
		{
			TRACE("GM Compile error '%s': %s\n", file, msg );
			//MESSAGE_BOX("GM Compile Error", "'%s': %s\n", file, msg ); 
		}
		compileLog.Reset();

#if defined(GM_COMPILE_ERROR_WAIT_FILETOUCH)

        FileModificationChecker checker = FileModificationChecker(file);

        while (checker.IsModified() == false)
        {
			// wait just a tad
			const int num_sleep_ms = 100;
			std::this_thread::sleep_for(std::chrono::milliseconds(num_sleep_ms));
        }

        checker.Sync();
#endif
		/*
		// wait
		clock_t goal = 1000+clock(); 
		while( goal > clock() ) {;}*/
	}
}

void OutputTableNode( std::ofstream &fh, gmTableObject * table, gmVariable & key, int level )
{
	// check not infinite loop
	const int recursionThreshold = 10;
	CHECK( level < recursionThreshold, "Saving table node file exceeded %d levels!", recursionThreshold );

	const char * TAB = "    ";

	for( int i = 0; i < level; ++i ) fh << TAB;

	gmVariable var = table->Get(key);
	gmType type = var.m_type;

	// output table key
	if ( key.IsString() ) fh << key.GetCStringSafe() << " = ";

	switch( type )
	{
		case GM_NULL: fh << "null"; break;
		case GM_INT: fh << var.GetInt(); break;
		case GM_FLOAT: fh << var.GetFloat(); break;
		case GM_V2: 
		{
			funk::v2 vec = var.GetV2();
			fh << "v2(" << vec.x << ", " << vec.y << ")";
			break;
		}
		case GM_V2I: 
		{
			funk::v2i vec = var.GetV2i();
			fh << "v2i(" << vec.x << ", " << vec.y << ")";
			break;
		}
		case GM_V3: 
		{
			funk::v3 vec = var.GetV3();
			fh << "v3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
			break;
		}
		case GM_STRING: fh << '"' << var.GetCStringSafe() << '"'; break;

		// invalid ones
		case GM_FUNCTION: fh << "null, // function"; break;
		case GM_USER: fh << "null, // user-type"; break;

		case GM_TABLE:
		{
			if ( key.IsString() ) 
			{
				fh << std::endl;
				for( int i = 0; i < level; ++i ) fh << TAB;
			}
			fh << "{" << std::endl;

			gmTableObject * childTable = var.GetTableObjectSafe();
			std::vector<gmVariable*> tableKeys;

			gmSortTableKeys( childTable, tableKeys );
			for( size_t i = 0; i < tableKeys.size(); ++i )
			{
				OutputTableNode( fh, childTable, *tableKeys[i], level+1 );
			}

			break;
		}
	};

	if ( type == GM_TABLE )
	{
		for( int i = 0; i < level; ++i ) fh << TAB;
		fh << "}";
	}
	
	if ( type != GM_FUNCTION && type != GM_USER ) fh << ",";
	fh << std::endl;
}

gmTableObject* gmLoadTableFromFile( gmMachine* a_machine, const char * file )
{
	ASSERT( a_machine != nullptr );

	int thread = gmCompileStr( a_machine, file );
	gmVariable result_table = a_machine->GetGlobals()->Get( a_machine, GM_RESOURCE_GLOBAL_KEY );

	CHECK( !result_table.IsNull(), "'%s' not found in file '%s'", GM_RESOURCE_GLOBAL_KEY, file );

	// null the global value
	gmVariable null_var;
	null_var.Nullify();
	a_machine->GetGlobals()->Set( a_machine, GM_RESOURCE_GLOBAL_KEY, null_var );

	return result_table.GetTableObjectSafe();
}

int gmSaveTableToFile( gmTableObject * table, const char * file )
{
	GM_ASSERT(table);

	std::ofstream fh(file);
	fh.precision(6);
	std::showpoint(fh);

	if (!fh.is_open() )
	{
		CHECK( fh.is_open(), "Failed to load file '%s'", file );
		return 0;
	}
	
	// A* does not like this. does not handle levels well		
	fh << "// auto-generated file" << std::endl;
	fh << "global " GM_RESOURCE_GLOBAL_KEY" =" << std::endl << "{" << std::endl;

	std::vector<gmVariable*> tableKeys;
	gmSortTableKeys( table, tableKeys );
	for( size_t i = 0; i < tableKeys.size(); ++i )
	{
		OutputTableNode( fh, table, *tableKeys[i], 1 );
	}

	fh << "};";
	fh.close();

	TRACE("Saved table to file: '%s'\n", file );

	return 0;
}