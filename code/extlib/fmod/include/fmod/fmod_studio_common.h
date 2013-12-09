/*
    fmod_studio_common.h (Draft 2)
    Copyright (c), Firelight Technologies Pty, Ltd. 2012.

    This header defines common enumerations, structs and callbacks that are shared between the C and C++ interfaces.
*/

// This is considered confidential information. Please do not distribute!

#ifndef FMOD_STUDIO_COMMON_H
#define FMOD_STUDIO_COMMON_H

#include "fmod.h"

#if 0
// *** This section comes from fmod.h, and is only included here for clarity ***

typedef enum
{
    FMOD_OK,
    // ...
    FMOD_ERR_STUDIO_ASSET_NOT_FOUND
    // etc.
} FMOD_RESULT;

typedef struct
{
    float x;        /* X co-ordinate in 3D space. */
    float y;        /* Y co-ordinate in 3D space. */
    float z;        /* Z co-ordinate in 3D space. */
} FMOD_VECTOR;

typedef struct
{
    unsigned int   Data1;       /* Specifies the first 8 hexadecimal digits of the GUID */
    unsigned short Data2;       /* Specifies the first group of 4 hexadecimal digits.   */
    unsigned short Data3;       /* Specifies the second group of 4 hexadecimal digits.  */
    unsigned char  Data4[8];    /* Array of 8 bytes. The first 2 bytes contain the third group of 4 hexadecimal digits. The remaining 6 bytes contain the final 12 hexadecimal digits. */
} FMOD_GUID;

struct FMOD_SOUND;

// *** End fmod.h section ***
#endif

typedef unsigned int FMOD_STUDIO_INITFLAGS;

enum
{
    FMOD_STUDIO_INIT_NORMAL = 0x00000000,
    FMOD_STUDIO_INIT_LIVEUPDATE = 0x00000001
};

typedef struct FMOD_STUDIO_SYSTEM FMOD_STUDIO_SYSTEM;
typedef struct FMOD_STUDIO_EVENTDESCRIPTION FMOD_STUDIO_EVENTDESCRIPTION;
typedef struct FMOD_STUDIO_EVENTINSTANCE FMOD_STUDIO_EVENTINSTANCE;
typedef struct FMOD_STUDIO_EVENTINSTANCE_ITERATOR FMOD_STUDIO_EVENTINSTANCE_ITERATOR;
typedef struct FMOD_STUDIO_CUEINSTANCE FMOD_STUDIO_CUEINSTANCE;
typedef struct FMOD_STUDIO_PARAMETERINSTANCE FMOD_STUDIO_PARAMETERINSTANCE;
typedef struct FMOD_STUDIO_MIXERSTRIP FMOD_STUDIO_MIXERSTRIP;
typedef struct FMOD_STUDIO_BANK FMOD_STUDIO_BANK;

typedef enum
{
    FMOD_STUDIO_LOAD_BEGIN_NOW,
    FMOD_STUDIO_LOAD_PROHIBITED
} FMOD_STUDIO_LOADING_MODE;

typedef enum
{
    FMOD_STUDIO_UNLOAD_FAIL_ON_DEPENDENTS,
    FMOD_STUDIO_UNLOAD_FORCE_UNLOAD_DEPENDENTS
} FMOD_STUDIO_UNLOADING_MODE;

typedef enum
{
    FMOD_STUDIO_LOADING_STATE_UNLOADING,
    FMOD_STUDIO_LOADING_STATE_UNLOADED,
    FMOD_STUDIO_LOADING_STATE_LOADING,
    FMOD_STUDIO_LOADING_STATE_LOADED
} FMOD_STUDIO_LOADING_STATE;

typedef struct
{
    float minimum;
    float maximum;
} FMOD_STUDIO_PARAMETER_DESCRIPTION;

typedef enum
{
    FMOD_STUDIO_USER_PROPERTY_TYPE_INTEGER,
    FMOD_STUDIO_USER_PROPERTY_TYPE_BOOLEAN,
    FMOD_STUDIO_USER_PROPERTY_TYPE_FLOAT,
    FMOD_STUDIO_USER_PROPERTY_TYPE_STRING
} FMOD_STUDIO_USER_PROPERTY_TYPE;

typedef struct
{
    const char* name;
    FMOD_STUDIO_USER_PROPERTY_TYPE type;

    union
    {
        int intValue;
        bool boolValue;
        float floatValue;
        const char* stringValue;
    };
} FMOD_STUDIO_USER_PROPERTY;

struct FMOD_STUDIO_EVENT_INSTANCE_HANDLE;

typedef enum
{
    FMOD_STUDIO_EVENT_CALLBACK_STARTED,                     // Called when an instance starts. Parameters = FMOD_STUDIO_EVENT_INSTANCE_HANDLE.
    FMOD_STUDIO_EVENT_CALLBACK_STOPPED,                     // Called when an instance stops. Parameters = FMOD_STUDIO_EVENT_INSTANCE_HANDLE.
    FMOD_STUDIO_EVENT_CALLBACK_STOLEN,                      // Called when an instance is stopped by "steal oldest" or "steal quietest" behaviour. Parameters = FMOD_STUDIO_EVENT_INSTANCE_HANDLE.
    FMOD_STUDIO_EVENT_CALLBACK_CREATE_PROGRAMMER_SOUND,     // Called when a programmer sound needs to be created in order to play a programmer instrument. Parameters = FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES.
    FMOD_STUDIO_EVENT_CALLBACK_DESTROY_PROGRAMMER_SOUND     // Called when a programmer sound needs to be destroyed. Parameters = FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES.
} FMOD_STUDIO_EVENT_CALLBACK_TYPE;

typedef struct
{
    const char* name;                          // The name of the programmer instrument (set in FMOD Studio).
    FMOD_STUDIO_EVENT_INSTANCE_HANDLE* event;  // The event instance that needs to play the sound.
    FMOD_SOUND* sound;                         // The programmer-created sound. This should be filled in by the create callback, and cleaned up by the destroy callback.
} FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES;

typedef FMOD_RESULT (F_CALLBACK *FMOD_STUDIO_EVENT_CALLBACK)(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, void* parameters);

typedef enum
{
    FMOD_STUDIO_PLAYBACK_PLAYING,
    FMOD_STUDIO_PLAYBACK_IDLE,
    FMOD_STUDIO_PLAYBACK_SUSTAINING,
    FMOD_STUDIO_PLAYBACK_STOPPED
} FMOD_STUDIO_PLAYBACK_STATE;

typedef enum
{
    FMOD_STUDIO_STOP_ALLOWFADEOUT,
    FMOD_STUDIO_STOP_IMMEDIATE
} FMOD_STUDIO_STOP_MODE;

#endif // FMOD_STUDIO_COMMON_H
