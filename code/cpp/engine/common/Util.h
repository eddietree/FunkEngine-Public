
#pragma once

#include <stdint.h>

namespace funk
{
	inline uint32_t HashString( const char* a_str )
	{
		uint32_t hash = 0;
		while( *a_str != '\0')
		{
			hash = 65599*hash + *a_str;
			++a_str;
		}
		
		//size_t len = strlen(a_str);
		//for(size_t i = 0; i < len; ++i)	hash = 65599 * hash + a_str[i];
		
		return hash ^ (hash >> 16);
	}
	
	inline uint64_t HashUint32Pair( uint32_t a_val0, uint32_t a_val1 )
	{
		return uint64_t(a_val0) | (uint64_t(a_val1)<<32);
	}
	
	inline uint64_t HashStringPair( const char* a_str0, const char* a_str1 )
	{
		uint32_t hash_0 = HashString(a_str0);
		uint32_t hash_1 = HashString(a_str1);
		
		return HashUint32Pair(hash_0, hash_1);
	}
}
