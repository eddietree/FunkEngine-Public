#ifndef _INCLUDE_GM_BIND_HEADER_H_
#define _INCLUDE_GM_BIND_HEADER_H_

class gmMachine;

// Put in header file
#define GM_BIND_TYPEID(TYPE) static int s_gmUserTypeId;
#define GM_BIND_DECL( TYPE ) void gmBind##TYPE##Lib( gmMachine * a_machine );

#endif