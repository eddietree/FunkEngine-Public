#ifndef _INCLUDE_GM_UTIL_EX_H_
#define _INCLUDE_GM_UTIL_EX_H_

class gmMachine;
class gmTableObject;
struct gmVariable;

bool gmFileExists( const char * file );

int gmCompileStr( gmMachine *vm, const char* file );
int gmSaveTableToFile( gmTableObject * table, const char * file );
gmTableObject* gmLoadTableFromFile( gmMachine* a_machine, const char * file );

#endif