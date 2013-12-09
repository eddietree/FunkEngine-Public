
#pragma once

#include "Debug.h"
#include "StrongHandle.h"

namespace funk
{
	template< typename T >
	class HandledObj
	{
	public:
		HandledObj();
		
		StrongHandle<T> GetHandle();
		
		inline int	RefCount() const { return m_refCount; }
		inline void AddRef();
		inline void ReleaseRef();
		
	private:
		
		int m_refCount;
	};
	
	///////////////////////////////////////////////////////////////////////////////
	
	template< typename T >
	HandledObj<T>::HandledObj() : m_refCount(0)
	{;}
	
	template< typename T >
	void HandledObj<T>::AddRef()
	{
		++m_refCount;
		ASSERT(m_refCount >= 0);
	}
	
	template< typename T >
	void HandledObj<T>::ReleaseRef()
	{
		--m_refCount;
		ASSERT(m_refCount >= 0);
	}
	
	template< typename T >
	StrongHandle<T> HandledObj<T>::GetHandle()
	{
		ASSERT(m_refCount >= 0);
		return StrongHandle<T>((T*)this);
	}
}
