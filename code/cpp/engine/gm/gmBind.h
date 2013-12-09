#ifndef _INCLUDE_GM_BIND_H_
#define _INCLUDE_GM_BIND_H_

#include "gmMachine.h"
#include "gmThread.h"
#include "gmBindFuncGen.h"

#define GM_TYPEID(TYPE) (TYPE::s_gmUserTypeId)

///////////////////////////////////////////////////////////////////////////////

// Call this in gmMachine to register custom type
#define GM_BIND_INIT( TYPE, MACHINE ) gmBind##TYPE##Lib( MACHINE );

///////////////////////////////////////////////////////////////////////////////

// Put in header file
//#define GM_BIND_TYPEID(TYPE) static gmType s_gmUserTypeId;
//#define GM_BIND_DECL( TYPE ) void gmBind##TYPE##Lib( gmMachine * a_machine );

///////////////////////////////////////////////////////////////////////////////

// Put in cpp file
#define GM_BIND_DEFINE( TYPE )							\
	void gmBind##TYPE##Lib( gmMachine * a_machine ) {	\
		gmBindUser##TYPE##Type(a_machine);  \
		gmBindUser##TYPE##LibFuncs(a_machine); }

// default MEMBER BEGIN
#define GM_REG_MEM_BEGIN( TYPE )								\
	void gmBindUser##TYPE##Type( gmMachine * a_machine ) {				\
		gmFunctionEntry gm##TYPE##ConstructorEntry[] = { #TYPE, gmf##TYPE::gmfCreate##TYPE, 0 };		\
		a_machine->RegisterLibrary( gm##TYPE##ConstructorEntry, 1 );	\
		GM_TYPEID(TYPE) = a_machine->CreateUserType(#TYPE); }	\
	int TYPE::s_gmUserTypeId = 0;								\
	void gmBindUser##TYPE##LibFuncs(gmMachine * a_machine) {	\
		gmType gmTypeId = GM_TYPEID(TYPE);

// MEMBER begin with NO constructor..
#define GM_REG_MEM_BEGIN_NO_CONSTRUCTOR( TYPE )								\
	void gmBindUser##TYPE##Type( gmMachine * a_machine ) {				\
		GM_TYPEID(TYPE) = a_machine->CreateUserType(#TYPE); }	\
	int TYPE::s_gmUserTypeId = 0;								\
	void gmBindUser##TYPE##LibFuncs(gmMachine * a_machine) {	\
		gmType gmTypeId = GM_TYPEID(TYPE);

#define GM_REG_NAMESPACE( CLASS ) namespace gmf##CLASS
#define GM_MEMFUNC_DECL( FUNC_NAME ) static int GM_CDECL gmf##FUNC_NAME( gmThread *a_thread )
#define GM_MEMFUNC_CONSTRUCTOR( CLASS ) GM_MEMFUNC_DECL( Create##CLASS )
#define GM_REG_MEMFUNC( CLASS, FUNC_NAME ) a_machine->RegisterTypeLibraryFunction( gmTypeId, #FUNC_NAME, gmf##CLASS::gmf##FUNC_NAME);

#define GM_REG_MEM_END() }


///////////////////////////////////////////////////////////////////////////////

// Pushing Funk HandledObj items
#define GM_PUSH_USER_HANDLED( TYPE, PTR ) { TYPE* _ptr = (PTR); _ptr->AddRef(); GM_THREAD_ARG->PushNewUser( _ptr, GM_TYPEID(TYPE) ); }

///////////////////////////////////////////////////////////////////////////////

// Default class Destructors
#define GM_REG_DEFAULT_DESTRUCTORS( TYPE ) a_machine->RegisterUserCallbacks(GM_TYPEID(TYPE), gmDefaultTrace<TYPE>, gmDestructDefault<TYPE>, NULL);
#define GM_REG_HANDLED_DESTRUCTORS( TYPE ) a_machine->RegisterUserCallbacks(GM_TYPEID(TYPE), gmDefaultTrace<TYPE>, gmDestructStrongHandled<TYPE>, NULL);

template<class T>
void GM_CDECL gmDestructDefault(gmMachine * a_machine, gmUserObject* a_object)
{
	GM_ASSERT( a_object->m_userType == GM_TYPEID(T) );
	T* p = (T*)(a_object->m_user);
	delete p;
}

template<class T>
void GM_CDECL gmDestructStrongHandled(gmMachine * a_machine, gmUserObject* a_object)
{
	GM_ASSERT( a_object->m_userType == GM_TYPEID(T) );
	T* p = (T*)(a_object->m_user);

	// handled obj release
	p->ReleaseRef();
	if ( p->RefCount() == 0 ) delete p;
}

template< class T >
bool GM_CDECL gmDefaultTrace( gmMachine * a_machine, 
					  gmUserObject* a_object, 
					  gmGarbageCollector* a_gc, 
					  const int a_workLeftToGo, 
					  int& a_workDone )
{
	// Make sure this is the correct type
	GM_ASSERT(a_object->m_userType == GM_TYPEID(T));

	// If your object immediately references other gmObjects, you should call GetNextObject for each member here

	// mark 'me' as done
	a_workDone++;
	return true;
}


///////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG
#define GM_GET_THIS_PTR(TYPE, PTR) TYPE * PTR = (TYPE*)a_thread->ThisUser_NoChecks();
#define GM_GET_THIS_CONST_PTR(TYPE, PTR) const TYPE * PTR = (TYPE*)a_thread->ThisUser_NoChecks();
#else
#define GM_GET_THIS_PTR(TYPE, PTR) TYPE * PTR = (TYPE*)a_thread->ThisUserCheckType(GM_TYPEID(TYPE));
#define GM_GET_THIS_CONST_PTR(TYPE, PTR) const TYPE * PTR = (TYPE*)a_thread->ThisUserCheckType(GM_TYPEID(TYPE));
#endif

///////////////////////////////////////////////////////////////////////////////
#endif