#pragma once

#include "Debug.h"

/*
 Template class for all Singleton classes
 */
template <class T>
class Singleton
{
public:
	static void CreateInst();
	static void DestroyInst();
	
	inline static bool InstExists();
	inline static T *Get( void );
	inline static T &Ref( void );
	
protected:
	static T *s_pInst;
};

template <class T>
inline void Singleton<T>::CreateInst()
{
	if (!s_pInst) s_pInst = new T();
}

template <class T>
inline T & Singleton<T>::Ref( void )
{
	// Not instantiated yet!
	ASSERT( s_pInst != nullptr );
	
	return *s_pInst;
}

template <class T>
inline void Singleton<T>::DestroyInst()
{
	if (s_pInst != nullptr)
	{
		delete s_pInst;
		s_pInst = nullptr;
	}
}

template <class T>
inline T *Singleton<T>::Get(void)
{
	return s_pInst;
}

template <class T>
inline bool Singleton<T>::InstExists()
{
	return s_pInst != nullptr;
}

template <class T>
T *Singleton<T>::s_pInst = nullptr;