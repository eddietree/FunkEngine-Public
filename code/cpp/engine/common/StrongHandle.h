
#pragma once

namespace funk
{
	template< typename T >
	class StrongHandle
	{
	public:
		StrongHandle( T* pVal = 0 );
		StrongHandle( const StrongHandle& ptr );
		~StrongHandle();
		
		inline T*		Get() const { return m_pVal; }
		inline T&		Ref() const { return *m_pVal; }
		inline bool		IsNull() const;
		
		// Pointer ops
		inline T*		operator->();
		inline const T*	operator->() const;
		inline T&		operator*();
		inline const T&	operator*() const;
		
		// Casting
		inline operator T*();
		inline operator const T*() const;
		
		// Copy ops
		StrongHandle& operator=( T* pVal );
		StrongHandle& operator=( const StrongHandle& ptr );
		
		// Comparison ops
		bool operator==( const StrongHandle& other ) const;
		bool operator==( const T* pVal ) const;
		bool operator!=( const StrongHandle& other ) const;
		bool operator!=( const T* pVal ) const;
		
	private:
		
		// Holds the data
		T * m_pVal;
		
		inline void AddRef();
		inline void ReleaseRef();
	};
	
	///////////////////////////////////////////////////////////////////////////////
	
	template< typename T >
	StrongHandle<T>::StrongHandle( T* pVal ) : m_pVal( pVal )
	{
		AddRef();
	}
	
	template< typename T >
	StrongHandle<T>::StrongHandle( const StrongHandle& ptr )
	{
		m_pVal = ptr.m_pVal;
		AddRef();
	}
	
	template< typename T >
	StrongHandle<T>::~StrongHandle()
	{
		ReleaseRef();
	}
	
	template< typename T >
	bool StrongHandle<T>::IsNull() const
	{
		return m_pVal == 0;
	}
	
	template< typename T >
	void StrongHandle<T>::AddRef()
	{
		if ( m_pVal ) m_pVal->AddRef();
	}
	
	template< typename T >
	void StrongHandle<T>::ReleaseRef()
	{
		if ( m_pVal )
		{
			m_pVal->ReleaseRef();
			
			// Check to see if can delete
			if ( m_pVal->RefCount() == 0 )
			{
				delete m_pVal;
				m_pVal = 0;
			}
		}
	}
	
	template< typename T >
	T* StrongHandle<T>::operator->()
	{
		return m_pVal;
	}
	
	template< typename T >
	const T* StrongHandle<T>::operator->() const
	{
		return m_pVal;
	}
	
	template< typename T >
	T& StrongHandle<T>::operator*()
	{
		return *m_pVal;
	}
	template< typename T >
	const T& StrongHandle<T>::operator*() const
	{
		return *m_pVal;
	}
	
	template< typename T >
	StrongHandle<T>& StrongHandle<T>::operator=( T* pVal )
	{
		// Ignore if same
		if ( pVal == m_pVal ) return *this;
		
		ReleaseRef();
		m_pVal = pVal;
		AddRef();
		
		return *this;
	}
	
	template< typename T >
	StrongHandle<T>& StrongHandle<T>::operator=( const StrongHandle& ptr )
	{
		if ( ptr.m_pVal == m_pVal) return *this;
		
		ReleaseRef();
		m_pVal = ptr.m_pVal;
		AddRef();
		
		return *this;
	}
	
	template< typename T >
	StrongHandle<T>::operator T*()
	{
		return m_pVal;
	}
	
	template< typename T >
	StrongHandle<T>::operator const T*() const
	{
		return m_pVal;
	}
	
	template< typename T >
	bool StrongHandle<T>::operator==( const StrongHandle& other ) const
	{
		return m_pVal == other.m_pVal;
	}
	
	template< typename T >
	bool StrongHandle<T>::operator==( const T* pVal ) const
	{
		return m_pVal == pVal;
	}
	
	template< typename T >
	bool StrongHandle<T>::operator!=( const StrongHandle& other ) const
	{
		return !( other == *this );
	}
	
	template< typename T >
	bool StrongHandle<T>::operator!=( const T* pVal ) const
	{
		return !( pVal == *this );
	}
	
	}
