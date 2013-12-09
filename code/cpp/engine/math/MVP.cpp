#include "MVP.h"

namespace funk
{
	MVP::MVP()
	{
		LOG("MVP initialized");
	}
	
	MVP::~MVP()
	{}
	
	matrix MVP::Get()
	{
		if ( Model().IsDirty() || View().IsDirty() || Proj().IsDirty() )
		{
			matrix view_inv = View().Get().inverseRT();
			m_mvp_cache =  Proj().Get() * view_inv * Model().Get();
		}
		
		return m_mvp_cache;
	}
}