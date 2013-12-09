#include "FmodEventDesc.h"

#if defined(FMOD_ENABLED)

#include "FmodSystem.h"
#include "FmodEventInst.h"

#include <gm/gmBind.h>
#include <common/Debug.h>

namespace funk
{
FmodEventDesc::FmodEventDesc() : m_num_user_properties(0), m_valid(false)
{
}

FmodEventDesc::~FmodEventDesc()
{
	Release();
}

void FmodEventDesc::Release()
{
	if ( m_valid )
	{
		// release instances
		for( auto it : m_instances ) it->Release();
		m_instances.clear();

		TRACE( "FmodEventDesc::Release() - %s\n", m_guid.c_str() );
		FMOD_ERROR_CHECK( m_desc.releaseAllInstances() );

		m_valid = false;
	}
}

void FmodEventDesc::Init( const char * a_guid )
{
	TRACE( "FmodEventDesc::Init() - %s\n", a_guid );

	m_guid = a_guid;
	m_valid = true;

	FMOD_ERROR_CHECK( m_desc.getUserPropertyCount(&m_num_user_properties) );
	FMOD_ERROR_CHECK( m_desc.getLength(&m_length) );
	FMOD_ERROR_CHECK( m_desc.getMaximumDistance(&m_max_distance) );
	FMOD_ERROR_CHECK( m_desc.isOneshot(&m_is_oneshot) );
	FMOD_ERROR_CHECK( m_desc.is3D(&m_is_3d) );
}

StrongHandle<FmodEventInst> FmodEventDesc::SpawnInst()
{
	ASSERT(m_valid);

	StrongHandle<FmodEventInst> result = new FmodEventInst;

	// init the instance
	FMOD::Studio::EventInstance & inst = result->GetInst();
	FMOD_ERROR_CHECK( m_desc.createInstance( &inst ) );

	result->Init( GetGuid().c_str(), this->GetHandle() );
	RegisterInst(result);

	return result;
}

gmVariable FmodEventDesc::GetUserProperty( gmMachine* a_machine, const char* a_name )
{
	ASSERT(m_valid);

	FMOD_STUDIO_USER_PROPERTY prop;
	FMOD_RESULT result = m_desc.getUserProperty( a_name, &prop );

	if ( result != FMOD_OK )
	{
		WARN( result == FMOD_OK, "FmodEvent: unable to get user property '%s'", a_name );
		gmVariable null_var;
		null_var.Nullify();
		return null_var;
	}

	switch( prop.type )
	{
		case FMOD_STUDIO_USER_PROPERTY_TYPE_FLOAT: return gmVariable(prop.floatValue);
		case FMOD_STUDIO_USER_PROPERTY_TYPE_INTEGER: return gmVariable(prop.intValue);
		case FMOD_STUDIO_USER_PROPERTY_TYPE_BOOLEAN: return gmVariable(prop.boolValue?1:0);
		
		case FMOD_STUDIO_USER_PROPERTY_TYPE_STRING: 
		{
			gmStringObject* obj = a_machine->AllocStringObject( prop.stringValue );
			return gmVariable(obj);
		}
	}
	
	// should not get here
	gmVariable var;
	var.Nullify();
	return var;
}


float FmodEventDesc::GetUserPropertyFloat( const char * a_name ) const
{
	FMOD_STUDIO_USER_PROPERTY prop;
	if ( m_desc.getUserProperty( a_name, &prop ) != FMOD_OK ) return 0.0f;
	ASSERT( prop.type == FMOD_STUDIO_USER_PROPERTY_TYPE_FLOAT );
	return prop.floatValue;
}

int	FmodEventDesc::GetUserPropertyInt( const char * a_name ) const
{
	FMOD_STUDIO_USER_PROPERTY prop;
	if ( m_desc.getUserProperty( a_name, &prop ) != FMOD_OK ) return 0;
	ASSERT( prop.type == FMOD_STUDIO_USER_PROPERTY_TYPE_INTEGER );
	return prop.intValue;
}

bool FmodEventDesc::GetUserPropertyBool( const char * a_name ) const	
{
	FMOD_STUDIO_USER_PROPERTY prop;
	if ( m_desc.getUserProperty( a_name, &prop ) != FMOD_OK ) return false;
	ASSERT( prop.type == FMOD_STUDIO_USER_PROPERTY_TYPE_BOOLEAN );
	return prop.boolValue;
}

const char* FmodEventDesc::GetUserPropertyString( const char * a_name ) const
{
	FMOD_STUDIO_USER_PROPERTY prop;
	if ( m_desc.getUserProperty( a_name, &prop ) != FMOD_OK ) return "";
	ASSERT( prop.type == FMOD_STUDIO_USER_PROPERTY_TYPE_STRING );
	return prop.stringValue;
}

void FmodEventDesc::RegisterInst( StrongHandle<FmodEventInst> a_inst )
{
	ASSERT(m_valid);
	ASSERT( a_inst != nullptr );
	m_instances.push_back( a_inst );
}

void FmodEventDesc::UnregisterInst( StrongHandle<FmodEventInst> a_inst )
{
	ASSERT(m_valid);

	for( size_t i = 0; i < m_instances.size(); ++i )
	{
		// found
		if ( a_inst == m_instances[i] )
		{
			if ( m_instances.size() == 1 ) // just one left, just clear
			{
				m_instances.clear();
			}
			else
			{
				// swap with end
				m_instances[i] = m_instances.back();
				m_instances.resize( m_instances.size()-1 );
			}
		}
	}

	// should not reach here
	ASSERT(false);
}

GM_REG_NAMESPACE(FmodEventDesc)
{
	GM_MEMFUNC_CONSTRUCTOR(FmodEventDesc)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( a_event_guid, 0 );

		StrongHandle<FmodEventDesc> p = FmodSystem::Ref().CreateEventDesc(a_event_guid);

		if ( p == nullptr ) 
		{ 
			a_thread->PushNull();
		}
		else
		{
			GM_PUSH_USER_HANDLED( FmodEventDesc, p.Get() );
		}

		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetUserProperty)
	{
		GM_CHECK_NUM_PARAMS(1);
		GM_CHECK_STRING_PARAM( a_name, 0);
		GM_GET_THIS_PTR(FmodEventDesc, ptr);

		gmVariable var = ptr->GetUserProperty(a_thread->GetMachine(), a_name);
		a_thread->Push(var);

		return GM_OK;
	}

	GM_MEMFUNC_DECL(SpawnInst)
	{
		GM_CHECK_NUM_PARAMS(0);
		GM_GET_THIS_PTR(FmodEventDesc, ptr);

		StrongHandle<FmodEventInst> inst = ptr->SpawnInst();
		GM_PUSH_USER_HANDLED( FmodEventInst, inst.Get() );

		return GM_OK;
	}

	GM_GEN_MEMFUNC_VOID_VOID( FmodEventDesc, Release )
	GM_GEN_MEMFUNC_INT_VOID( FmodEventDesc, GetNumUserProperties )
	GM_GEN_MEMFUNC_INT_VOID( FmodEventDesc, GetLength )
	GM_GEN_MEMFUNC_FLOAT_VOID( FmodEventDesc, GetMaxDistance )
	GM_GEN_MEMFUNC_BOOL_VOID( FmodEventDesc, IsOneShot )
	GM_GEN_MEMFUNC_BOOL_VOID( FmodEventDesc, Is3d )
}

GM_REG_MEM_BEGIN(FmodEventDesc)
GM_REG_MEMFUNC( FmodEventDesc, Release )
GM_REG_MEMFUNC( FmodEventDesc, SpawnInst )
GM_REG_MEMFUNC( FmodEventDesc, GetNumUserProperties )
GM_REG_MEMFUNC( FmodEventDesc, GetLength )
GM_REG_MEMFUNC( FmodEventDesc, GetMaxDistance )
GM_REG_MEMFUNC( FmodEventDesc, IsOneShot )
GM_REG_MEMFUNC( FmodEventDesc, Is3d )
GM_REG_MEMFUNC( FmodEventDesc, GetUserProperty )
GM_REG_HANDLED_DESTRUCTORS(FmodEventDesc)
GM_REG_MEM_END()
GM_BIND_DEFINE(FmodEventDesc)

}

#endif // FMOD_ENABLED