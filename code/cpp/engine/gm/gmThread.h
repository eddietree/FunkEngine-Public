/*
    _____               __  ___          __            ____        _      __
   / ___/__ ___ _  ___ /  |/  /__  ___  / /_____ __ __/ __/_______(_)__  / /_
  / (_ / _ `/  ' \/ -_) /|_/ / _ \/ _ \/  '_/ -_) // /\ \/ __/ __/ / _ \/ __/
  \___/\_,_/_/_/_/\__/_/  /_/\___/_//_/_/\_\\__/\_, /___/\__/_/ /_/ .__/\__/
                                               /___/             /_/
                                             
  See Copyright Notice in gmMachine.h

*/

#ifndef _GMTHREAD_H_
#define _GMTHREAD_H_

#include "gmConfig.h"
#include "gmListDouble.h"
#include "gmHash.h"
#include "gmVariable.h"
#include "gmMachine.h"
#include "gmStringObject.h"
#include "gmUserObject.h"

#include <math/v2.h>
#include <math/v3.h>

class gmFunctionObject;
class gmStringObject;
class gmTableObject;
class gmUserObject;
class gmBlock;
struct gmSignal;

#define GMTHREAD_SLACKSPACE  6 // floating slack space above tos for operator calls
#define GM_INVALID_THREAD 0

/// \struct gmStackFrame
/// \brief The stack order is as follows: this, fp, p0..pn-1, l0..ln-1.  gmStackFrame objects are allocated from the gmMachine.
///        Base pointer is at the first parameter.
struct gmStackFrame
{
  gmStackFrame * m_prev;
  const gmuint8 * m_returnAddress;
  int m_returnBase;
};

/// \class gmThread
/// \brief gmThread.. try to keep this class's memory footprint small.. at the time of this comment, its
///        76 bytes.
class gmThread : public gmListDoubleNode<gmThread>, public gmHashNode<int, gmThread>
{
public:
  gmThread(gmMachine * a_machine, int a_initialByteSize = GMTHREAD_INITIALBYTESIZE);
  virtual ~gmThread();

  enum State
  {
    RUNNING = 0,
    SLEEPING,
    BLOCKED,
    KILLED,

    EXCEPTION, //!< exception state, for debugging

    SYS_PENDING,
    SYS_YIELD, 
    SYS_EXCEPTION,
  };

  inline const int GetKey() const { return m_id; }

#if GM_USE_INCGC
  void GCScanRoots(gmMachine* a_machine, gmGarbageCollector* a_gc);
#else //GM_USE_INCGC
  /// \brief Mark() will call Mark() for all objects in the stack.
  void Mark(gmuint32 a_mark);
#endif //GM_USE_INCGC

  /// \brief Sys_Execute() will perform execution on this thread.  a this, function references, params and stack
  ///        frame must be pushed before a call to execute will succeed.
  /// \param a_return will be set to the return variable iff Sys_Execute returns gmThread::KILLED. 
  /// \return the new thread state.
  State Sys_Execute(gmVariable * a_return = NULL);

  /// \brief Sys_Reset() will reset the thread.
  void Sys_Reset(int a_id);

  /// \brief Sys_SetState() will set the thread state.
  inline void Sys_SetState(State a_state) { m_state = a_state; }

  /// \brief PushStackFrame will push a stack frame and adjust the instruction and code pointers. 
  ///        If the function to be called is a c bound function, the call will occur within PushStackFrame.
  ///        Before PushStackFrame is called, this, fp, and params must be pushed on the stack.
  /// \param a_numParameters is the number of params pushed after the function ref.
  /// \param a_ip is the current instruction pointer (pointing to instruction after call.) will be adjusted for new function.
  /// \param a_cp is the current code pointer.  will be adjusted for the new function. a_cp MUST be valid if a_ip is valid.
  /// \return gmThreadState
  State PushStackFrame(int a_numParameters, const gmuint8 ** a_ip = NULL, const gmuint8 ** a_cp = NULL);

  /// \brief GetTop() will return the top of stack
  inline gmVariable * GetTop() const { return &m_stack[m_top]; }

  /// \brief GetBottom() will return the bottom of stact
  inline gmVariable * GetBottom() const { return m_stack; }

  /// \brief SetTop() will set the top of stack
  inline void SetTop(gmVariable * a_top) { m_top = (int)(a_top - m_stack); }

  /// \brief GetBase() will return the current stack base.
  inline gmVariable * GetBase() const { return &m_stack[m_base]; }
  inline int GetIntBase() const { return m_base; }

  /// \brief GetMachine() will return the stacks parent machine.
  inline gmMachine * GetMachine() const { return m_machine; }

  /// \brief GetThis() will return the this variable for the current stack frame.
  inline const gmVariable * GetThis() const { return &m_stack[m_base - 2]; }

  /// \brief GetFunction() will return the current executing function.
  inline const gmVariable * GetFunction() const { return &m_stack[m_base - 1]; }
  inline const gmFunctionObject * GetFunctionObject() const;

  /// \brief GetNumParams() returns the number parameters pushed into the current cFunction call.
  inline int GetNumParams() const { return m_numParameters; }

  /// \brief GetFrame() will return the top stack frame.
  inline const gmStackFrame * GetFrame() const { return m_frame; }

  //
  // Push methods
  //

  inline gmVariable &Pop() { return m_stack[--m_top]; }
  inline void Push(const gmVariable &a_variable) { m_stack[m_top++] = a_variable; }
  inline void PushNull();
  inline void PushInt(gmint a_value);
  inline void PushBool(bool a_value) { PushInt(a_value?1:0); }
  inline void PushFloat(gmfloat a_value);
  inline void PushV2(funk::v2 a_value);
  inline void PushV2i(funk::v2i a_value);
  inline void PushV3(funk::v3 a_value);
  inline void PushString(gmStringObject * a_string);
  inline void PushTable(gmTableObject * a_table);
  inline void PushFunction(gmFunctionObject * a_function);
  inline void PushUser(gmUserObject * a_user);

  inline gmStringObject * PushNewString(const char * a_value, int a_len = -1); //!< PushNewString() will push a new string object onto tos.
  inline gmTableObject * PushNewTable(); //!< PushNewTable() will push a new table onto tos.
  inline gmUserObject * PushNewUser(void * a_user, int a_userType); //!< PushNewUser() will push a new user object onto tos.

  //
  // Parameter methods. (do not cause an error if the desired parameter is incorrect type)
  // Versions that return bool return false if type was invalid, otherwise true, even if param was out of range. NOTE: Could switch to more complex return code, but user could simply check num params for range check if needed.
  // 

  inline int ParamInt(int a_param, gmint a_default = 0) const;
  inline bool ParamInt(int a_param, gmint& a_value, gmint a_default = 0) const;
  inline gmfloat ParamFloat(int a_param, gmfloat a_default = 0.0f) const;
  inline bool ParamFloat(int a_param, gmfloat& a_value, gmfloat a_default = 0.0f) const;
  inline gmfloat ParamFloatOrInt(int a_param, gmfloat a_default = 0.0f) const;
  inline bool ParamFloatOrInt(int a_param, gmfloat& a_value, gmfloat a_default = 0.0f) const;
  inline gmVec2 ParamV2(int a_param, funk::v2 a_default=funk::v2(0.0f)) const;
  inline bool ParamV2(int a_param, funk::v2& a_value, funk::v2 a_default=funk::v2(0.0f)) const;
  inline gmVec2i ParamV2i(int a_param, funk::v2i a_default=funk::v2i(0)) const;
  inline bool ParamV2i(int a_param, funk::v2i& a_value, funk::v2i a_default=funk::v2i(0)) const;
  inline gmVec3 ParamV3(int a_param, funk::v3 a_default=funk::v3(0.0f)) const;
  inline bool ParamV3(int a_param, funk::v3& a_value, funk::v3 a_default=funk::v3(0.0f)) const;
  inline const char * ParamString(int a_param, const char * a_default = "") const;
  inline bool ParamString(int a_param, const char *& a_value, const char * a_default = "") const;
  inline gmStringObject * ParamStringObject(int a_param) const;
  inline bool ParamStringObject(int a_param, gmStringObject *& a_value) const;
  inline gmTableObject * ParamTable(int a_param) const;
  inline bool ParamTable(int a_param, gmTableObject *& a_value ) const;
  inline gmFunctionObject * ParamFunction(int a_param) const;
  inline bool ParamFunction(int a_param, gmFunctionObject *& a_value) const;
  inline void * ParamUser(int a_param, int * a_userType = NULL) const;
  inline bool ParamUser(int a_param, void *& a_value, int * a_userType = NULL) const;
  inline void * ParamUserCheckType(int a_param, int a_userType) const;
  inline void * ParamUser_NoCheckTypeOrParam(int a_param) const; //For macro
  inline gmUserObject * ParamUserObject(int a_param) const;
  inline bool ParamUserObject(int a_param, gmUserObject *& a_value) const;
  inline gmVariable& Param(int a_param);
  inline const gmVariable& Param(int a_param) const;
  inline gmVariable Param(int a_param, const gmVariable& a_default);  
  inline gmType ParamType(int a_param) const;
  inline gmptr ParamRef(int a_param) const;

  //
  // This methods. (get this as a given type)
  //

  inline gmptr ThisInt(gmptr a_default = 0);
  inline gmfloat ThisFloat(gmfloat a_default = 0.0f);
  inline gmVec2 ThisV2();
  inline gmVec2i ThisV2i();
  inline gmVec3 ThisV3();
  inline const char * ThisString(const char * a_default = "");
  inline gmTableObject * ThisTable();
  inline gmFunctionObject * ThisFunction();
  inline void * ThisUser(int * a_userType = NULL);
  inline void * ThisUserCheckType(int a_userType);
  inline void * ThisUser_NoChecks(); //For use with type functions where type is already certain
  inline gmUserObject * ThisUserObject();

  /// \brief Touch() will make sure that you can push a_extra variables on the stack
  /// \return true on success
  bool Touch(int a_extra);

  inline int GetId() const { return m_id; }
  inline const gmuint8 * GetInstruction() const { return m_instruction; }
  inline State GetState() const { return m_state; }
  inline gmuint32 GetTimeStamp() const { return m_timeStamp; }
  inline gmuint32 GetStartTime() const { return m_startTime; }
  inline gmuint32 GetThreadTime() const { return m_machine->GetTime() - m_startTime; }
  inline void Sys_SetTimeStamp(gmuint32 a_timeStamp) { m_timeStamp = a_timeStamp; }
  inline void Sys_SetStartTime(gmuint32 a_startTime) { m_startTime = a_startTime; }

  /// \brief GetSystemMemUsed will return the number of bytes allocated by the system.
  inline unsigned int GetSystemMemUsed() const { return (m_size * sizeof(gmVariable)) + sizeof(this); }

  // Eddie: thread groups
  inline int  GetGroup() { return m_groupId; }
  inline void SetGroup(int group) { m_groupId = group; }

  void LogCallStack();

  /// \brief Sys_SetBlocks() will set the blocking list for the thread.
  inline void Sys_SetBlocks(gmBlock * a_block) { m_blocks = a_block; }

  /// \brief Sys_GetBlocks() will return the list of blocks on a thread
  inline gmBlock * Sys_GetBlocks() { return m_blocks; }

  /// \brief Sys_SetSignals() will set the signal list for this thread
  inline void Sys_SetSignals(gmSignal * a_signals) { m_signals = a_signals; }

  /// \brief Sys_GetSignals() will return the list of signals on a thread
  inline gmSignal * Sys_GetSignals() { return m_signals; }

  // public data
#if GMDEBUG_SUPPORT
  mutable int m_debugFlags; //!< non-zero when thread is being debugged
  mutable int m_debugUser;
#endif // GMDEBUG_SUPPORT
  mutable int m_user;

private:

  /// \brief Sys_PopStackFrame() will pop a frame off the stack.
  /// \param a_ip is the current instruction pointer (pointing to instruction after call.) will be adjusted for new function.
  /// \param a_cp is the current code pointer.  will be adjusted for the new function.
  /// \return RUNNING, KILLED or SYS_EXCEPTION
  State Sys_PopStackFrame(const gmuint8 * &a_ip, const gmuint8 * &a_cp);

  // EDDIE
  State Sys_ExecuteMainLoop(gmVariable * a_return);

  void LogLineFile();

  // stack members
  gmMachine * m_machine;
  gmVariable * m_stack;
  int m_size;
  int m_top;
  int m_base;
  gmStackFrame * m_frame;

  // thread members
  State m_state;
  gmuint32 m_timeStamp; // wake up at this time stamp.
  gmuint32 m_startTime; // time this thread was started.
  const gmuint8 * m_instruction;
  int m_id;
  int m_groupId;
  gmSignal * m_signals; // list of potentially active signals on this thread.
  gmBlock * m_blocks; // list of active blocks when thread is in BLOCKED state.
  short m_numParameters;
};


//
//
// INLINE IMPLEMENTATION
//
//


inline const gmFunctionObject * gmThread::GetFunctionObject() const
{
  const gmVariable * fnVar = GetFunction();
  GM_ASSERT(fnVar->m_type == GM_FUNCTION);
  return (gmFunctionObject *) m_machine->GetObject(fnVar->m_value.m_ref);
}

//
// Push methods
//

inline void gmThread::PushNull()
{
  m_stack[m_top].m_type = GM_NULL;
  m_stack[m_top++].m_value.m_int = 0;
}


inline void gmThread::PushInt(gmint a_value)
{
  m_stack[m_top].m_type = GM_INT;
  m_stack[m_top++].m_value.m_int = a_value;
}


inline void gmThread::PushFloat(gmfloat a_value)
{
  m_stack[m_top].m_type = GM_FLOAT;
  m_stack[m_top++].m_value.m_float = a_value;
}

inline void gmThread::PushV2(funk::v2 a_value)
{
	m_stack[m_top].m_type = GM_V2;
	m_stack[m_top++].m_value.m_v2 = a_value.gmv2;
}

inline void gmThread::PushV2i(funk::v2i a_value)
{
	m_stack[m_top].m_type = GM_V2I;
	m_stack[m_top++].m_value.m_v2i = a_value.gmv2i;
}

inline void gmThread::PushV3(funk::v3 a_value)
{
	m_stack[m_top].m_type = GM_V3;
	m_stack[m_top++].m_value.m_v3 = a_value.gmv3;
}

inline void gmThread::PushString(gmStringObject * a_string)
{
  m_stack[m_top].m_type = GM_STRING;
  m_stack[m_top++].m_value.m_ref = (gmptr) a_string;
}


inline void gmThread::PushTable(gmTableObject * a_table)
{
  m_stack[m_top].m_type = GM_TABLE;
  m_stack[m_top++].m_value.m_ref = (gmptr) a_table;
}


inline void gmThread::PushFunction(gmFunctionObject * a_function)
{
  m_stack[m_top].m_type = GM_FUNCTION;
  m_stack[m_top++].m_value.m_ref = (gmptr) a_function;
}


inline void gmThread::PushUser(gmUserObject * a_user)
{
  m_stack[m_top].m_type = (gmType)a_user->GetType();
  m_stack[m_top++].m_value.m_ref = a_user->GetRef();
}


gmStringObject * gmThread::PushNewString(const char * a_value, int a_len)
{
  m_stack[m_top].m_type = GM_STRING;
  return (gmStringObject *) (m_stack[m_top++].m_value.m_ref = (gmptr) m_machine->AllocStringObject(a_value, a_len));
}


gmTableObject * gmThread::PushNewTable()
{
  m_stack[m_top].m_type = GM_TABLE;
  return (gmTableObject *) (m_stack[m_top++].m_value.m_ref = (gmptr) m_machine->AllocTableObject());
}


gmUserObject * gmThread::PushNewUser(void * a_user, int a_userType)
{
  m_stack[m_top].m_type = (gmType) a_userType;
  return (gmUserObject *) (m_stack[m_top++].m_value.m_ref = (gmptr) m_machine->AllocUserObject(a_user, a_userType));
}

//
// Parameter methods. (do not cause an error if the desired parameter is incorrect type)
//

inline int gmThread::ParamInt(int a_param, gmint a_default) const
{
  if(a_param >= m_numParameters) return a_default;
  gmVariable * var = m_stack + m_base + a_param;
  if(var->m_type == GM_INT) return var->m_value.m_int;
  return a_default;
}

inline bool gmThread::ParamInt(int a_param, gmint& a_value, gmint a_default) const
{
  // Out of range
  if( a_param >= m_numParameters )
  {
    a_value = a_default;
    return true;
  }
  // Valid
  gmVariable * var = m_stack + m_base + a_param;
  if( var->m_type == GM_INT )
  {
    a_value = var->m_value.m_int;
    return true;
  }
  // Invalid
  a_value = a_default;
  return false;
}


inline gmVec2 gmThread::ParamV2(int a_param, funk::v2 a_default ) const
{
	const gmVec2 default_val = {a_default.x, a_default.y};

	if(a_param >= m_numParameters) return default_val;
	gmVariable * var = m_stack + m_base + a_param;
	if(var->m_type == GM_V2) return var->m_value.m_v2;
	return default_val;
}

inline bool gmThread::ParamV2(int a_param, funk::v2& a_value, funk::v2 a_default ) const
{
	const gmVec2 default_val = {a_default.x, a_default.y};

	// Out of range
	if( a_param >= m_numParameters )
	{
		a_value = default_val;
		return true;
	}
	// Valid
	gmVariable * var = m_stack + m_base + a_param;
	if( var->m_type == GM_V2 )
	{
		a_value = var->m_value.m_v2;
		return true;
	}
	// Invalid
	a_value = default_val;
	return false;
}

inline gmVec2i gmThread::ParamV2i(int a_param, funk::v2i a_default ) const
{
	const gmVec2i default_val = {a_default.x, a_default.y};

	if(a_param >= m_numParameters) return default_val;
	gmVariable * var = m_stack + m_base + a_param;
	if(var->m_type == GM_V2I) return var->m_value.m_v2i;
	return default_val;
}


inline bool gmThread::ParamV2i(int a_param, funk::v2i& a_value, funk::v2i a_default ) const
{
	const gmVec2i default_val = {a_default.x, a_default.y};

	// Out of range
	if( a_param >= m_numParameters )
	{
		a_value = default_val;
		return true;
	}
	// Valid
	gmVariable * var = m_stack + m_base + a_param;
	if( var->m_type == GM_V2I )
	{
		a_value = var->m_value.m_v2i;
		return true;
	}
	// Invalid
	a_value = default_val;
	return false;
}

inline gmVec3 gmThread::ParamV3(int a_param, funk::v3 a_default) const
{
	const gmVec3 default_val = {a_default.x,a_default.y,a_default.z};

	if(a_param >= m_numParameters) return default_val;
	gmVariable * var = m_stack + m_base + a_param;
	if(var->m_type == GM_V3) return var->m_value.m_v3;
	return default_val;
}
inline bool gmThread::ParamV3(int a_param, funk::v3& a_value, funk::v3 a_default) const
{
	const gmVec3 default_val = {a_default.x,a_default.y,a_default.z};

	// Out of range
	if( a_param >= m_numParameters )
	{
		a_value = default_val;
		return true;
	}
	// Valid
	gmVariable * var = m_stack + m_base + a_param;
	if( var->m_type == GM_V3 )
	{
		a_value = var->m_value.m_v3;
		return true;
	}
	// Invalid
	a_value = default_val;
	return false;
}


inline gmfloat gmThread::ParamFloat(int a_param, gmfloat a_default) const
{
  if(a_param >= m_numParameters) return a_default;
  gmVariable * var = m_stack + m_base + a_param;
  if(var->m_type == GM_FLOAT) return var->m_value.m_float;
  return a_default;
}

inline bool gmThread::ParamFloat(int a_param, gmfloat& a_value, gmfloat a_default) const
{
  // Out of range
  if( a_param >= m_numParameters )
  {
    a_value = a_default;
    return true;
  }
  // Valid
  gmVariable * var = m_stack + m_base + a_param;
  if( var->m_type == GM_FLOAT )
  {
    a_value = var->m_value.m_float;
    return true;
  }
  // Invalid
  a_value = a_default;
  return false;
}

inline gmfloat gmThread::ParamFloatOrInt(int a_param, gmfloat a_default) const
{
  if(a_param >= m_numParameters) return a_default;
  gmVariable * var = m_stack + m_base + a_param;
  if(var->m_type == GM_FLOAT)
    { return var->m_value.m_float; }
  else if(var->m_type == GM_INT)
    { return (gmfloat)var->m_value.m_int; }
  return a_default;
}

inline bool gmThread::ParamFloatOrInt(int a_param, gmfloat& a_value, gmfloat a_default) const
{
  // Out of range
  if( a_param >= m_numParameters )
  {
    a_value = a_default;
    return true;
  }
  // Valid
  gmVariable * var = m_stack + m_base + a_param;
  if( var->m_type == GM_FLOAT )
  { 
    a_value = var->m_value.m_float; 
    return true;
  }
  else if(var->m_type == GM_INT)
  {
    a_value = (gmfloat)var->m_value.m_int; 
    return true;
  }
  // Invalid
  a_value = a_default;
  return false;
}

const char * gmThread::ParamString(int a_param, const char * a_default) const
{
  if(a_param >= m_numParameters) return a_default;
  gmVariable * var = m_stack + m_base + a_param;
  if(var->m_type == GM_STRING)
  {
    return (const char *) *((gmStringObject *) m_machine->GetObject(var->m_value.m_ref));
  }
  return a_default;
}

bool gmThread::ParamString(int a_param, const char *& a_value, const char * a_default) const
{
  // Out of range
  if( a_param >= m_numParameters)
  {
    a_value = a_default;
    return true;
  }
  // Valid
  gmVariable * var = m_stack + m_base + a_param;
  if( var->m_type == GM_STRING )
  {
    a_value = (const char *) *((gmStringObject *) m_machine->GetObject(var->m_value.m_ref));
    return true;
  }
  // Invalid
  a_value = a_default;
  return false;
}


gmStringObject * gmThread::ParamStringObject(int a_param) const
{
  if(a_param >= m_numParameters) return NULL;
  gmVariable * var = m_stack + m_base + a_param;
  if(var->m_type == GM_STRING)
  {
    return (gmStringObject *) m_machine->GetObject(var->m_value.m_ref);
  }
  return NULL;
}

bool gmThread::ParamStringObject(int a_param, gmStringObject *& a_value) const
{
  // Out of range
  if( a_param >= m_numParameters )
  {
    a_value = NULL;
    return true;
  }
  // Valid
  gmVariable * var = m_stack + m_base + a_param;
  if(var->m_type == GM_STRING)
  {
    a_value = (gmStringObject *) m_machine->GetObject(var->m_value.m_ref);
    return true;
  }
  // Invalid
  a_value = NULL;
  return false;
}

gmTableObject * gmThread::ParamTable(int a_param) const
{
  if(a_param >= m_numParameters) return NULL;
  gmVariable * var = m_stack + m_base + a_param;
  if(var->m_type == GM_TABLE)
  {
    return (gmTableObject *) m_machine->GetObject(var->m_value.m_ref);
  }
  return NULL;
}

bool gmThread::ParamTable(int a_param, gmTableObject *& a_value ) const
{
  // Out of range
  if( a_param >= m_numParameters )
  {
    a_value = NULL;
    return true;
  }
  // Valid
  gmVariable * var = m_stack + m_base + a_param;
  if( var->m_type == GM_TABLE )
  {
    a_value = (gmTableObject *) m_machine->GetObject(var->m_value.m_ref);
    return true;
  }
  // Invalid
  a_value = NULL;
  return false;
}


gmFunctionObject * gmThread::ParamFunction(int a_param) const
{
  if(a_param >= m_numParameters) return NULL;
  gmVariable * var = m_stack + m_base + a_param;
  if(var->m_type == GM_FUNCTION)
  {
    return (gmFunctionObject *) m_machine->GetObject(var->m_value.m_ref);
  }
  return NULL;
}


bool gmThread::ParamFunction(int a_param, gmFunctionObject *& a_value) const
{
  // Out of range
  if( a_param >= m_numParameters )
  {
    a_value = NULL;
    return true;
  }
  // Valid
  gmVariable * var = m_stack + m_base + a_param;
  if( var->m_type == GM_FUNCTION )
  {
    a_value = (gmFunctionObject *) m_machine->GetObject(var->m_value.m_ref);
    return true;
  }
  // Invalid
  a_value = NULL;
  return false;
}

void * gmThread::ParamUser(int a_param, int * a_userType) const
{
  if(a_param >= m_numParameters) return NULL;
  gmVariable * var = m_stack + m_base + a_param;
  if(var->m_type >= GM_USER)
  {
    gmUserObject * user = (gmUserObject *) m_machine->GetObject(var->m_value.m_ref);
    GM_ASSERT(var->m_type == user->m_userType);
    if(a_userType) 
    {
      *a_userType = user->m_userType;
    }
    return user->m_user;
  }
  return NULL;
}

bool gmThread::ParamUser(int a_param, void *& a_value, int * a_userType) const
{
  // Out of range
  if( a_param >= m_numParameters )
  {
    a_value = NULL;
    return true;
  }
  // Valid
  gmVariable * var = m_stack + m_base + a_param;
  if( var->m_type >= GM_USER )
  {
    gmUserObject * user = (gmUserObject *) m_machine->GetObject(var->m_value.m_ref);
    GM_ASSERT( var->m_type == user->m_userType );
    if( a_userType ) 
    {
      *a_userType = user->m_userType;
    }
    a_value = user->m_user;
    return true;
  }
  // Invalid
  a_value = NULL;
  return false;
}

void * gmThread::ParamUserCheckType(int a_param, int a_userType) const
{
  if(a_param >= m_numParameters) return NULL;
  gmVariable * var = m_stack + m_base + a_param;
  if(a_userType == var->m_type) 
  {
    GM_ASSERT(var->m_type >= GM_USER);
    gmUserObject * user = (gmUserObject *) m_machine->GetObject(var->m_value.m_ref);
    GM_ASSERT(var->m_type == user->m_userType);
    return user->m_user;
  }
  return NULL;
}


void * gmThread::ParamUser_NoCheckTypeOrParam(int a_param) const
{
  GM_ASSERT(a_param < m_numParameters);
  gmVariable * var = m_stack + m_base + a_param;
  GM_ASSERT(var->m_type >= GM_USER);
  gmUserObject * user = (gmUserObject *) m_machine->GetObject(var->m_value.m_ref);
  return user->m_user;
}


gmUserObject * gmThread::ParamUserObject(int a_param) const
{
  if(a_param >= m_numParameters) return NULL;
  gmVariable * var = m_stack + m_base + a_param;
  if(var->m_type >= GM_USER)
  {
    return (gmUserObject *) m_machine->GetObject(var->m_value.m_ref);
  }
  return NULL;
}


bool gmThread::ParamUserObject(int a_param, gmUserObject *& a_value) const
{
  // Out of range
  if( a_param >= m_numParameters )
  {
    a_value = NULL;
    return true;
  }
  // Valid
  gmVariable * var = m_stack + m_base + a_param;
  if( var->m_type >= GM_USER )
  {
    a_value = (gmUserObject *) m_machine->GetObject(var->m_value.m_ref);
    return true;
  }
  // Invalid
  a_value = NULL;
  return false;
}

inline gmVariable& gmThread::Param(int a_param)
{
  return *(m_stack + m_base + a_param);
}


inline const gmVariable& gmThread::Param(int a_param) const
{
  return *(m_stack + m_base + a_param);
}

gmVariable gmThread::Param(int a_param, const gmVariable& a_default)
{
  if( a_param >= m_numParameters )
  {
    return a_default;
  }
  return *(m_stack + m_base + a_param);
}

inline gmType gmThread::ParamType(int a_param) const
{
  return (m_stack + m_base + a_param)->m_type;
}


inline gmptr gmThread::ParamRef(int a_param) const
{
  return (m_stack + m_base + a_param)->m_value.m_ref;
}


//
// This methods.
//

inline gmptr gmThread::ThisInt(gmptr a_default)
{
  const gmVariable * var = GetThis();
  if(var->m_type == GM_INT) return var->m_value.m_int;
  return a_default;
}


inline gmfloat gmThread::ThisFloat(gmfloat a_default)
{
  const gmVariable * var = GetThis();
  if(var->m_type == GM_FLOAT) return var->m_value.m_float;
  return a_default;
}

inline gmVec2 gmThread::ThisV2()
{
	const gmVec2 default_val = {0,0};

	const gmVariable * var = GetThis();
	if(var->m_type == GM_V2) return var->m_value.m_v2;
	return default_val;
}

inline gmVec2i gmThread::ThisV2i()
{
	const gmVec2i default_val = {0,0};

	const gmVariable * var = GetThis();
	if(var->m_type == GM_V2I) return var->m_value.m_v2i;
	return default_val;
}

inline gmVec3 gmThread::ThisV3()
{
	const gmVec3 default_val = {0,0,0};

	const gmVariable * var = GetThis();
	if(var->m_type == GM_V3) return var->m_value.m_v3;
	return default_val;
}

inline const char * gmThread::ThisString(const char * a_default)
{
  const gmVariable * var = GetThis();
  if(var->m_type == GM_STRING)
  {
    return (const char *) *((gmStringObject *) m_machine->GetObject(var->m_value.m_ref));
  }
  return a_default;
}


inline gmTableObject * gmThread::ThisTable()
{
  const gmVariable * var = GetThis();
  if(var->m_type == GM_TABLE)
  {
    return (gmTableObject *) m_machine->GetObject(var->m_value.m_ref);
  }
  return NULL;
}


inline gmFunctionObject * gmThread::ThisFunction()
{
  const gmVariable * var = GetThis();
  if(var->m_type == GM_FUNCTION)
  {
    return (gmFunctionObject *) m_machine->GetObject(var->m_value.m_ref);
  }
  return NULL;
}


inline void * gmThread::ThisUser(int * a_userType)
{
  const gmVariable * var = GetThis();
  if(var->m_type >= GM_USER)
  {
    gmUserObject * user = (gmUserObject *) m_machine->GetObject(var->m_value.m_ref);
    if(a_userType) *a_userType = user->m_userType;
    return user->m_user;
  }
  return NULL;
}


inline void * gmThread::ThisUserCheckType(int a_userType)
{
  const gmVariable * var = GetThis();
  if(var->m_type >= GM_USER)
  {
    gmUserObject * user = (gmUserObject *) m_machine->GetObject(var->m_value.m_ref);
    if(a_userType != user->m_userType) 
    {
      return NULL;
    }
    return user->m_user;
  }
  return NULL;
}


inline void * gmThread::ThisUser_NoChecks()
{
  return ((gmUserObject*)m_machine->GetObject(GetThis()->m_value.m_ref))->m_user;
}


inline gmUserObject * gmThread::ThisUserObject()
{
  const gmVariable * var = GetThis();
  if(var->m_type >= GM_USER)
  {
    return (gmUserObject *) m_machine->GetObject(var->m_value.m_ref);
  }
  return NULL;
}

//
//
// Helper macros
//
//

#define GM_THREAD_ARG a_thread

#ifdef GM_DEBUG_BUILD
  #define GM_OBJECT(A) GM_THREAD_ARG->GetMachine()->GetObject((A))
  #define GM_MOBJECT(M, A) (M)->GetObject((A))
#else //GM_DEBUG_BUILD
  #if GMMACHINE_SUPERPARANOIDGC
    #define GM_OBJECT(A) GM_THREAD_ARG->GetMachine()->GetObject((A))
    #define GM_MOBJECT(M, A) (M)->GetObject((A))
  #else //GMMACHINE_SUPERPARANOIDGC
    #define GM_OBJECT(A) (gmObject *) (A)
    #define GM_MOBJECT(M, A) (gmObject *) (A)
  #endif //GMMACHINE_SUPERPARANOIDGC
#endif //GM_DEBUG_BUILD

#define GM_NUM_PARAMS GM_THREAD_ARG->GetNumParams()
#if 1 // These macros only exception if param is present but type does not match
  #define GM_INT_PARAM(VAR, PARAM, DEFAULT) int VAR; if( !GM_THREAD_ARG->ParamInt((PARAM), (VAR), (DEFAULT)) ) { return GM_EXCEPTION; }
  #define GM_FLOAT_PARAM(VAR, PARAM, DEFAULT) float VAR; if( !GM_THREAD_ARG->ParamFloat((PARAM), (VAR), (DEFAULT)) )  { return GM_EXCEPTION; }
  #define GM_V2_PARAM(VAR, PARAM) funk::v2 VAR; if( !GM_THREAD_ARG->ParamV2((PARAM), (VAR), v2(0.0f)) )  { return GM_EXCEPTION; }
  #define GM_V2I_PARAM(VAR, PARAM) funk::v2i VAR; if( !GM_THREAD_ARG->ParamV2i((PARAM), (VAR), v2i(0)) )  { return GM_EXCEPTION; }
  #define GM_V3_PARAM(VAR, PARAM) funk::v3 VAR; if( !GM_THREAD_ARG->ParamV3((PARAM), (VAR), v3(0.0f)) )  { return GM_EXCEPTION; }
  #define GM_STRING_PARAM(VAR, PARAM, DEFAULT) const char * VAR; if( !GM_THREAD_ARG->ParamString((PARAM), (VAR), (DEFAULT)) )  { return GM_EXCEPTION; }
  #define GM_FUNCTION_PARAM(VAR, PARAM) gmFunctionObject * VAR; if( !GM_THREAD_ARG->ParamFunction((PARAM), (VAR)) )  { return GM_EXCEPTION; }
  #define GM_TABLE_PARAM(VAR, PARAM) gmTableObject * VAR; if( !GM_THREAD_ARG->ParamTable((PARAM), (VAR))  )  { return GM_EXCEPTION; }
  #define GM_USER_PARAM(OBJECT, VAR, PARAM) OBJECT VAR; if( !GM_THREAD_ARG->ParamUser((PARAM), (void*&)(VAR)) )  { return GM_EXCEPTION; }
  #define GM_FLOAT_OR_INT_PARAM(VAR, PARAM, DEFAULT) float VAR; if( !GM_THREAD_ARG->ParamFloatOrInt((PARAM), (VAR), (DEFAULT)) ) { return GM_EXCEPTION; }
#else // Old versions
  #define GM_INT_PARAM(VAR, PARAM, DEFAULT) int VAR = GM_THREAD_ARG->ParamInt((PARAM), (DEFAULT))
  #define GM_FLOAT_PARAM(VAR, PARAM, DEFAULT) float VAR = GM_THREAD_ARG->ParamFloat((PARAM), (DEFAULT))
  #define GM_STRING_PARAM(VAR, PARAM, DEFAULT) const char * VAR = GM_THREAD_ARG->ParamString((PARAM), (DEFAULT))
  #define GM_FUNCTION_PARAM(VAR, PARAM) gmFunctionObject * VAR = GM_THREAD_ARG->ParamFunction((PARAM))
  #define GM_TABLE_PARAM(VAR, PARAM) gmTableObject * VAR = GM_THREAD_ARG->ParamTable((PARAM))
  #define GM_USER_PARAM(OBJECT, VAR, PARAM) OBJECT VAR = (OBJECT) GM_THREAD_ARG->ParamUser((PARAM));
  #define GM_FLOAT_OR_INT_PARAM(VAR, PARAM, DEFAULT) float VAR = GM_THREAD_ARG->ParamFloatOrInt((PARAM), (DEFAULT))
#endif

//
// OPERATORS
//

#define GM_OP_USER_PTR( TYPE, INDEX, PTR ) GM_OP_ASSERT( a_operands[INDEX].m_type == GM_TYPEID(TYPE), "userptr" ); TYPE* PTR = static_cast<TYPE*>(static_cast< gmUserObject*>(GM_OBJECT(a_operands[INDEX].m_value.m_ref))->m_user);
#define GM_OP_STR_PTR( CHAR_PTR, INDEX) GM_OP_ASSERT( a_operands[INDEX].m_type == GM_STRING, "string" ); const char* CHAR_PTR = (static_cast< gmStringObject*>(GM_OBJECT(a_operands[INDEX].m_value.m_ref)))->GetString();
#define GM_OP_V2( VAL, INDEX ) GM_OP_ASSERT( a_operands[INDEX].m_type == GM_V2, "v2" ); gmVec2 &VAL = a_operands[INDEX].m_value.m_v2;
#define GM_OP_V2I( VAL, INDEX ) GM_OP_ASSERT( a_operands[INDEX].m_type == GM_V2I, "v2i" ); gmVec2i &VAL = a_operands[INDEX].m_value.m_v2i;
#define GM_OP_V3( VAL, INDEX ) GM_OP_ASSERT( a_operands[INDEX].m_type == GM_V3, "v3" ); gmVec3 &VAL = a_operands[INDEX].m_value.m_v3;
#define GM_OP_FLOAT( VAL, INDEX) GM_OP_ASSERT( a_operands[INDEX].m_type == GM_FLOAT, "float" ); float VAL = a_operands[INDEX].m_value.m_float;
#define GM_OP_INT( VAL, INDEX) GM_OP_ASSERT( a_operands[INDEX].m_type == GM_INT, "int" ); int VAL = a_operands[INDEX].m_value.m_int;
#define GM_OP_FLOAT_OR_INT( VAL, INDEX ) GM_OP_ASSERT( a_operands[INDEX].m_type == GM_FLOAT || a_operands[INDEX].m_type == GM_INT, "floatint" ); float VAL = (float)(a_operands[INDEX].m_type == GM_INT )?a_operands[INDEX].m_value.m_int:a_operands[INDEX].m_value.m_float;



//
// EXCEPTION VERSIONS
//

#define GM_EXCEPTION_MSG GM_THREAD_ARG->GetMachine()->GetLog().LogEntry

#define GM_CHECK_NUM_PARAMS(A) \
  if(GM_THREAD_ARG->GetNumParams() < (A)) { GM_EXCEPTION_MSG("expecting %d param(s)", (A)); return GM_EXCEPTION; }

#define GM_CHECK_NUM_PARAMS_RANGE(A,B) \
  if(GM_THREAD_ARG->GetNumParams() < (A) || GM_THREAD_ARG->GetNumParams() > (B)) { GM_EXCEPTION_MSG("expecting %d-%d param(s)", (A), (B)); return GM_EXCEPTION; }

#define GM_CHECK_NUM_PARAMS_EITHER(A,B) \
  if(GM_THREAD_ARG->GetNumParams() != (A) && GM_THREAD_ARG->GetNumParams() != (B)) { GM_EXCEPTION_MSG("expecting %d-%d param(s)", (A), (B)); return GM_EXCEPTION; }

#define GM_CHECK_INT_PARAM(VAR, PARAM) \
  if(GM_THREAD_ARG->ParamType((PARAM)) != GM_INT) { GM_EXCEPTION_MSG("expecting param %d as int", (PARAM)); return GM_EXCEPTION; } \
  int VAR = GM_THREAD_ARG->Param((PARAM)).m_value.m_int;

#define GM_CHECK_FLOAT_PARAM(VAR, PARAM) \
  if(GM_THREAD_ARG->ParamType((PARAM)) != GM_FLOAT) \
  { GM_EXCEPTION_MSG("expecting param %d as float", (PARAM)); return GM_EXCEPTION; } \
  float VAR = GM_THREAD_ARG->Param((PARAM)).m_value.m_float;

#define GM_CHECK_V2_PARAM(VAR, PARAM) \
	if(GM_THREAD_ARG->ParamType((PARAM)) != GM_V2) { GM_EXCEPTION_MSG("expecting param %d as v2", (PARAM)); return GM_EXCEPTION; } \
	funk::v2 VAR = GM_THREAD_ARG->Param((PARAM)).m_value.m_v2;

#define GM_CHECK_V2I_PARAM(VAR, PARAM) \
	if(GM_THREAD_ARG->ParamType((PARAM)) != GM_V2I) { GM_EXCEPTION_MSG("expecting param %d as v2i", (PARAM)); return GM_EXCEPTION; } \
	funk::v2i VAR = GM_THREAD_ARG->Param((PARAM)).m_value.m_v2i;

#define GM_CHECK_V3_PARAM(VAR, PARAM) \
	if(GM_THREAD_ARG->ParamType((PARAM)) != GM_V3) { GM_EXCEPTION_MSG("expecting param %d as v3", (PARAM)); return GM_EXCEPTION; } \
	funk::v3 VAR = GM_THREAD_ARG->Param((PARAM)).m_value.m_v3;

#define GM_CHECK_STRING_PARAM(VAR, PARAM) \
  if(GM_THREAD_ARG->ParamType((PARAM)) != GM_STRING) { GM_EXCEPTION_MSG("expecting param %d as string", (PARAM)); return GM_EXCEPTION; } \
  const char * VAR = (const char *) *((gmStringObject *) GM_OBJECT(GM_THREAD_ARG->ParamRef((PARAM))));

#define GM_CHECK_FUNCTION_PARAM(VAR, PARAM) \
  if(GM_THREAD_ARG->ParamType((PARAM)) != GM_FUNCTION) { GM_EXCEPTION_MSG("expecting param %d as function", (PARAM)); return GM_EXCEPTION; } \
  gmFunctionObject * VAR = (gmFunctionObject *) GM_OBJECT(GM_THREAD_ARG->ParamRef((PARAM)));

#define GM_CHECK_TABLE_PARAM(VAR, PARAM) \
  if(GM_THREAD_ARG->ParamType((PARAM)) != GM_TABLE) { GM_EXCEPTION_MSG("expecting param %d as table", (PARAM)); return GM_EXCEPTION; } \
  gmTableObject * VAR = (gmTableObject *) GM_OBJECT(GM_THREAD_ARG->ParamRef((PARAM)));

#define GM_CHECK_USER_PARAM(OBJECT, TYPE, VAR, PARAM) \
  if(GM_THREAD_ARG->ParamType((PARAM)) != (TYPE)) { GM_EXCEPTION_MSG("expecting param %d as user type %d", (PARAM), (TYPE)); return GM_EXCEPTION; } \
  OBJECT VAR = (OBJECT) GM_THREAD_ARG->ParamUser_NoCheckTypeOrParam((PARAM));

// funk!
#define GM_CHECK_USER_PARAM_PTR( TYPE, VAR, PARAM ) GM_CHECK_USER_PARAM( TYPE*, GM_TYPEID(TYPE), VAR, PARAM )
#define GM_GET_USER_PARAM_PTR( TYPE, VAR, PARAM ) TYPE* VAR = (TYPE*) GM_THREAD_ARG->ParamUser_NoCheckTypeOrParam((PARAM));
#define GM_IS_PARAM_USER_TYPE(TYPE, PARAM) ( GM_THREAD_ARG->Param(PARAM).m_type == GM_TYPEID(TYPE) )
#define GM_IS_PARAM_V2(PARAM) ( GM_THREAD_ARG->Param(PARAM).m_type == GM_V2 )
#define GM_IS_PARAM_V2I(PARAM) ( GM_THREAD_ARG->Param(PARAM).m_type == GM_V2I )
#define GM_IS_PARAM_V3(PARAM) ( GM_THREAD_ARG->Param(PARAM).m_type == GM_V3 )
#define GM_IS_PARAM_STRING(PARAM) ( GM_THREAD_ARG->Param(PARAM).m_type == GM_STRING )
#define GM_IS_PARAM_TABLE(PARAM) ( GM_THREAD_ARG->Param(PARAM).m_type == GM_TABLE )
#define GM_IS_PARAM_FLOAT(PARAM) ( GM_THREAD_ARG->Param(PARAM).m_type == GM_FLOAT )
#define GM_IS_PARAM_INT(PARAM) ( GM_THREAD_ARG->Param(PARAM).m_type == GM_INT )

#define GM_CHECK_THIS_NULL \
    if(GM_THREAD_ARG->GetThis()->m_type != GM_NULL) { GM_EXCEPTION_MSG("expecting this as null"); return GM_EXCEPTION; }

#define GM_CHECK_THIS_INT \
    if(GM_THREAD_ARG->GetThis()->m_type != GM_INT) { GM_EXCEPTION_MSG("expecting this as int"); return GM_EXCEPTION; }

#define GM_CHECK_THIS_FLOAT \
    if(GM_THREAD_ARG->GetThis()->m_type != GM_FLOAT) { GM_EXCEPTION_MSG("expecting this as null"); return GM_EXCEPTION; }

#define GM_CHECK_THIS_STRING \
    if(GM_THREAD_ARG->GetThis()->m_type != GM_STRING) { GM_EXCEPTION_MSG("expecting this as string"); return GM_EXCEPTION; }

#define GM_CHECK_THIS_TABLE \
    if(GM_THREAD_ARG->GetThis()->m_type != GM_TABLE) { GM_EXCEPTION_MSG("expecting this as table"); return GM_EXCEPTION; }

#define GM_CHECK_THIS_FUNCTION \
    if(GM_THREAD_ARG->GetThis()->m_type != GM_FUNCTION) { GM_EXCEPTION_MSG("expecting this as function"); return GM_EXCEPTION; }

#define GM_CHECK_THIS_USER(TYPE) \
    if(GM_THREAD_ARG->GetThis()->m_type != TYPE) { GM_EXCEPTION_MSG("expecting this as user type %d", TYPE); return GM_EXCEPTION; }
    
// float or int param as float variable
#define GM_CHECK_FLOAT_OR_INT_PARAM(VAR, PARAM) \
  float VAR; \
  if( GM_THREAD_ARG->ParamType((PARAM)) == GM_FLOAT ) \
    { VAR = GM_THREAD_ARG->Param((PARAM)).m_value.m_float; } \
  else if( GM_THREAD_ARG->ParamType((PARAM)) == GM_INT ) \
    { VAR = (gmfloat)GM_THREAD_ARG->Param((PARAM)).m_value.m_int; } \
  else \
    { GM_EXCEPTION_MSG("expecting param %d as float or int", (PARAM)); return GM_EXCEPTION; }

// v2 or v2i param as v2 variable
#define GM_CHECK_V2_OR_V2I_PARAM(VAR, PARAM) \
  v2 VAR; \
  if( GM_THREAD_ARG->ParamType((PARAM)) == GM_V2 ) \
    { VAR = v2(GM_THREAD_ARG->Param((PARAM)).m_value.m_v2); } \
  else if( GM_THREAD_ARG->ParamType((PARAM)) == GM_V2I ) \
    { VAR = v2( (float)GM_THREAD_ARG->Param((PARAM)).m_value.m_v2i.x, (float)GM_THREAD_ARG->Param((PARAM)).m_value.m_v2i.y); } \
  else \
    { GM_EXCEPTION_MSG("expecting param %d as v2 or v2i", (PARAM)); return GM_EXCEPTION; }

// NOTE: there maybe is a better place than this for these

#define GM_OP_ASSERT( cond, expected ) do { gmOpAssert( (cond), expected, a_thread, a_operands ); } while ( 0 )
void gmDumpThreadCallstack( gmThread* a_thread, int steps = 16 );
void gmOpAssert( bool cond, const char* expected, gmThread* a_thread, gmVariable* a_operand );

extern void gmThreadProfileBegin( gmThread* a_thread );
extern void gmThreadProfileEnd( gmThread* a_thread );

#endif // _GMTHREAD_H_
