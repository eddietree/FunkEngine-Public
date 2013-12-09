/*
    _____               __  ___          __            ____        _      __
   / ___/__ ___ _  ___ /  |/  /__  ___  / /_____ __ __/ __/_______(_)__  / /_
  / (_ / _ `/  ' \/ -_) /|_/ / _ \/ _ \/  '_/ -_) // /\ \/ __/ __/ / _ \/ __/
  \___/\_,_/_/_/_/\__/_/  /_/\___/_//_/_/\_\\__/\_, /___/\__/_/ /_/ .__/\__/
                                               /___/             /_/
                                             
  See Copyright Notice in gmMachine.h

*/

#include "gmConfig.h"
#include "gmUtil.h"
#include "gmVariable.h"
#include "gmTableObject.h"

static char s_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char * gmItoa(int a_val, char * a_dst, int a_radix)
{
  char * dst = a_dst;
  char buffer[65];
  char *p;
  int new_val;

  if (a_radix < 0) {
    if (a_radix < -36 || a_radix > -2) return NULL;
    if (a_val < 0) {
      *dst++ = '-';
      a_val = -a_val;
    }
    a_radix = -a_radix;
  } 
  else 
  {
    if (a_radix > 36 || a_radix < 2) return NULL;
  }
  p = &buffer[sizeof(buffer)-1];
  *p = '\0';
  new_val = (gmuint32) a_val / (gmuint32) a_radix;
  *--p = s_digits[(gmuint8) ((gmuint32) a_val- (gmuint32) new_val*(gmuint32) a_radix)];
  a_val = new_val;
  while (a_val != 0)
  {
    new_val=a_val/a_radix;
    *--p = s_digits[(gmuint8) (a_val-new_val*a_radix)];
    a_val= new_val;
  }

  while ((*dst++ = *p++) != 0) ;
  return a_dst;
}


bool gmVariableCmp( const gmVariable & v0, const gmVariable & v1 )
{
	if ( v0.m_type == v1.m_type )
	{
		gmType type = v0.m_type;

		switch(type)
		{
			case GM_INT: return v0.GetInt() < v1.GetInt();
			case GM_FLOAT: return v0.GetFloat() < v1.GetFloat();
			case GM_STRING: return strcmp( v0.GetCStringSafe(), v1.GetCStringSafe() ) < 0;
			default: return true;
		}
	}
	else
	{
		return v0.m_type < v1.m_type;
	}	

	GM_ASSERT(false);
	return false;
}

void gmSortTableKeys( gmTableObject * table, std::vector<gmVariable*> & result )
{
	int tableSize = table->Count();

	result.clear();

	if ( tableSize > 0 )
	{
		result.resize( tableSize );
		gmSortTableKeysRaw( table, &result[0] );
	}
}

void gmSortTableKeysRaw( gmTableObject* table, gmVariable** result )
{
	int tableSize = table->Count();

	gmTableIterator it;
	gmTableNode * node = table->GetFirst( it );
    int index = 0;
	while ( !table->IsNull(it) )
	{
		result[ index++ ] = &(node->m_key);
		node = table->GetNext(it);
	}

	// sort keys
	for( int i = 0; i < tableSize-1; ++i )
	{
		for( int j = i+1; j < tableSize; ++j )
		{
			if ( !gmVariableCmp( *result[i], *result[j] ) )
			{
				gmVariable * temp = result[i];
				result[i] = result[j];
				result[j] = temp;
			}
		}
	}
}