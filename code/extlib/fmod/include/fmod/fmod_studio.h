#ifndef _FMOD_STUDIO_H
#define _FMOD_STUDIO_H

#include "fmod_studio_common.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/*
    Global
*/
FMOD_RESULT F_API FMOD_Studio_ParseID(const char* idString, FMOD_GUID* id);
FMOD_RESULT F_API FMOD_Studio_System_Create(FMOD_STUDIO_SYSTEM** system);

/*
    System
*/
FMOD_RESULT F_API FMOD_Studio_System_Initialize(FMOD_STUDIO_SYSTEM* system, int maxchannels, FMOD_STUDIO_INITFLAGS studioFlags, FMOD_INITFLAGS flags, void *extradriverdata);
FMOD_RESULT F_API FMOD_Studio_System_Release(FMOD_STUDIO_SYSTEM* system);
FMOD_RESULT F_API FMOD_Studio_System_Update(FMOD_STUDIO_SYSTEM* system);
FMOD_RESULT F_API FMOD_Studio_System_GetLowLevelSystem(FMOD_STUDIO_SYSTEM* system, FMOD_SYSTEM** lowLevelSystem);
FMOD_RESULT F_API FMOD_Studio_System_GetEvent(FMOD_STUDIO_SYSTEM* system, FMOD_GUID* id, FMOD_STUDIO_LOADING_MODE mode, FMOD_STUDIO_EVENTDESCRIPTION** eventDesc);
FMOD_RESULT F_API FMOD_Studio_System_GetMixerStrip(FMOD_STUDIO_SYSTEM* system, const FMOD_GUID* id, FMOD_STUDIO_LOADING_MODE mode, FMOD_STUDIO_MIXERSTRIP** mixerStrip);
FMOD_RESULT F_API FMOD_Studio_System_GetBank(FMOD_STUDIO_SYSTEM* system, const FMOD_GUID* id, FMOD_STUDIO_BANK** bank);
FMOD_RESULT F_API FMOD_Studio_System_LookupEventID(FMOD_STUDIO_SYSTEM* system, const char* path, FMOD_GUID* id);
FMOD_RESULT F_API FMOD_Studio_System_LookupBusID(FMOD_STUDIO_SYSTEM* system, const char* path, FMOD_GUID* id);
FMOD_RESULT F_API FMOD_Studio_System_GetListenerAttributes(FMOD_STUDIO_SYSTEM* system, FMOD_3D_ATTRIBUTES* attributes);
FMOD_RESULT F_API FMOD_Studio_System_SetListenerAttributes(FMOD_STUDIO_SYSTEM* system, FMOD_3D_ATTRIBUTES* attributes);
FMOD_RESULT F_API FMOD_Studio_System_LoadBank(FMOD_STUDIO_SYSTEM* system, const char* filename, FMOD_STUDIO_BANK** bank);
FMOD_RESULT F_API FMOD_Studio_System_RegisterPlugin(FMOD_STUDIO_SYSTEM* system, const FMOD_DSP_DESCRIPTION* description);
FMOD_RESULT F_API FMOD_Studio_System_UnregisterPlugin(FMOD_STUDIO_SYSTEM* system, const char* name);
FMOD_RESULT F_API FMOD_Studio_System_UnloadAll(FMOD_STUDIO_SYSTEM* system);

/*
    EventDescription
*/
FMOD_RESULT F_API FMOD_Studio_EventDescription_GetID(FMOD_STUDIO_EVENTDESCRIPTION* eventDesc, FMOD_GUID* id);
FMOD_RESULT F_API FMOD_Studio_EventDescription_GetUserPropertyCount(FMOD_STUDIO_EVENTDESCRIPTION* eventDesc, int* count);
FMOD_RESULT F_API FMOD_Studio_EventDescription_GetUserPropertyByIndex(FMOD_STUDIO_EVENTDESCRIPTION* eventDesc, int index, FMOD_STUDIO_USER_PROPERTY* property);
FMOD_RESULT F_API FMOD_Studio_EventDescription_GetUserProperty(FMOD_STUDIO_EVENTDESCRIPTION* eventDesc, const char* name, FMOD_STUDIO_USER_PROPERTY* property);
FMOD_RESULT F_API FMOD_Studio_EventDescription_GetLength(FMOD_STUDIO_EVENTDESCRIPTION* eventDesc, int* length);
FMOD_RESULT F_API FMOD_Studio_EventDescription_GetMaximumDistance(FMOD_STUDIO_EVENTDESCRIPTION* eventDesc, float* distance);
FMOD_RESULT F_API FMOD_Studio_EventDescription_IsOneshot(FMOD_STUDIO_EVENTDESCRIPTION* eventDesc, FMOD_BOOL* oneshot);
FMOD_RESULT F_API FMOD_Studio_EventDescription_Is3D(FMOD_STUDIO_EVENTDESCRIPTION* eventDesc, FMOD_BOOL* is3D);        
FMOD_RESULT F_API FMOD_Studio_EventDescription_CreateInstance(FMOD_STUDIO_EVENTDESCRIPTION* eventDesc, FMOD_STUDIO_EVENTINSTANCE** event);
FMOD_RESULT F_API FMOD_Studio_EventDescription_EnumeratePersistentInstances(FMOD_STUDIO_EVENTDESCRIPTION* eventDesc, FMOD_STUDIO_EVENTINSTANCE_ITERATOR** iterator); 
FMOD_RESULT F_API FMOD_Studio_EventDescription_SetCallback(FMOD_STUDIO_EVENTDESCRIPTION* eventDesc, FMOD_STUDIO_EVENT_CALLBACK callback);
FMOD_RESULT F_API FMOD_Studio_EventDescription_GetUserData(FMOD_STUDIO_EVENTDESCRIPTION* eventDesc, void** userData);
FMOD_RESULT F_API FMOD_Studio_EventDescription_SetUserData(FMOD_STUDIO_EVENTDESCRIPTION* eventDesc, void* userData);   

/*
    EventInstance
*/
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetDescription(FMOD_STUDIO_EVENTINSTANCE* event, FMOD_STUDIO_EVENTDESCRIPTION** description);
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetVolume(FMOD_STUDIO_EVENTINSTANCE* event, float* volume);
FMOD_RESULT F_API FMOD_Studio_EventInstance_SetVolume(FMOD_STUDIO_EVENTINSTANCE* event, float volume);
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetPitch(FMOD_STUDIO_EVENTINSTANCE* event, float* pitch);
FMOD_RESULT F_API FMOD_Studio_EventInstance_SetPitch(FMOD_STUDIO_EVENTINSTANCE* event, float pitch);
FMOD_RESULT F_API FMOD_Studio_EventInstance_Get3DAttributes(FMOD_STUDIO_EVENTINSTANCE* event, FMOD_3D_ATTRIBUTES* attributes);
FMOD_RESULT F_API FMOD_Studio_EventInstance_Set3DAttributes(FMOD_STUDIO_EVENTINSTANCE* event, FMOD_3D_ATTRIBUTES* attributes);
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetPaused(FMOD_STUDIO_EVENTINSTANCE* event, FMOD_BOOL* paused);
FMOD_RESULT F_API FMOD_Studio_EventInstance_SetPaused(FMOD_STUDIO_EVENTINSTANCE* event, FMOD_BOOL paused);
FMOD_RESULT F_API FMOD_Studio_EventInstance_Start(FMOD_STUDIO_EVENTINSTANCE* event);
FMOD_RESULT F_API FMOD_Studio_EventInstance_Stop(FMOD_STUDIO_EVENTINSTANCE* event, FMOD_STUDIO_STOP_MODE mode);
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetTimelinePosition(FMOD_STUDIO_EVENTINSTANCE* event, int* position);
FMOD_RESULT F_API FMOD_Studio_EventInstance_SetTimelinePosition(FMOD_STUDIO_EVENTINSTANCE* event, int position);
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetPlaybackState(FMOD_STUDIO_EVENTINSTANCE* event, FMOD_STUDIO_PLAYBACK_STATE* state);
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetChannelGroup(FMOD_STUDIO_EVENTINSTANCE* event, FMOD_CHANNELGROUP** group);
FMOD_RESULT F_API FMOD_Studio_EventInstance_Release(FMOD_STUDIO_EVENTINSTANCE* event);
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetParameter(FMOD_STUDIO_EVENTINSTANCE* event, const char* name, FMOD_STUDIO_PARAMETERINSTANCE** parameter);
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetParameterByIndex(FMOD_STUDIO_EVENTINSTANCE* event, int index, FMOD_STUDIO_PARAMETERINSTANCE** parameter);
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetParameterCount(FMOD_STUDIO_EVENTINSTANCE* event, int* parameterCount);
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetCue(FMOD_STUDIO_EVENTINSTANCE* event, const char* name, FMOD_STUDIO_CUEINSTANCE** cue);
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetCueByIndex(FMOD_STUDIO_EVENTINSTANCE* event, int index, FMOD_STUDIO_CUEINSTANCE** cue);
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetCueCount(FMOD_STUDIO_EVENTINSTANCE* event, int* cueCount);
FMOD_RESULT F_API FMOD_Studio_EventInstance_CreateSubEvent(FMOD_STUDIO_EVENTINSTANCE* event, const char* name, FMOD_STUDIO_EVENTINSTANCE** instance);
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetLoadingState(FMOD_STUDIO_EVENTINSTANCE* event, FMOD_STUDIO_LOADING_STATE* state);
FMOD_RESULT F_API FMOD_Studio_EventInstance_SetCallback(FMOD_STUDIO_EVENTINSTANCE* event, FMOD_STUDIO_EVENT_CALLBACK callback);
FMOD_RESULT F_API FMOD_Studio_EventInstance_GetUserData(FMOD_STUDIO_EVENTINSTANCE* event, void** userData);
FMOD_RESULT F_API FMOD_Studio_EventInstance_SetUserData(FMOD_STUDIO_EVENTINSTANCE* event, void* userData);

/*
    CueInstance
*/
FMOD_RESULT F_API FMOD_Studio_CueInstance_Trigger(FMOD_STUDIO_CUEINSTANCE* cue);

/*
    ParameterInstance
*/
FMOD_RESULT F_API FMOD_Studio_ParameterInstance_GetValue(FMOD_STUDIO_PARAMETERINSTANCE* parameter, float* value);
FMOD_RESULT F_API FMOD_Studio_ParameterInstance_SetValue(FMOD_STUDIO_PARAMETERINSTANCE* parameter, float value);

/*
    MixerStrip
*/
FMOD_RESULT F_API FMOD_Studio_MixerStrip_GetFaderLevel(FMOD_STUDIO_MIXERSTRIP* strip, float* level);
FMOD_RESULT F_API FMOD_Studio_MixerStrip_SetFaderLevel(FMOD_STUDIO_MIXERSTRIP* strip, float level);
FMOD_RESULT F_API FMOD_Studio_MixerStrip_GetPaused(FMOD_STUDIO_MIXERSTRIP* strip, FMOD_BOOL* paused);
FMOD_RESULT F_API FMOD_Studio_MixerStrip_SetPaused(FMOD_STUDIO_MIXERSTRIP* strip, FMOD_BOOL paused);
FMOD_RESULT F_API FMOD_Studio_MixerStrip_StopAllEvents(FMOD_STUDIO_MIXERSTRIP* strip, FMOD_STUDIO_STOP_MODE mode);
FMOD_RESULT F_API FMOD_Studio_MixerStrip_GetChannelGroup(FMOD_STUDIO_MIXERSTRIP* strip, FMOD_CHANNELGROUP** channelgroup);
FMOD_RESULT F_API FMOD_Studio_MixerStrip_GetLoadingState(FMOD_STUDIO_MIXERSTRIP* strip, FMOD_STUDIO_LOADING_STATE* state);
FMOD_RESULT F_API FMOD_Studio_MixerStrip_Release(FMOD_STUDIO_MIXERSTRIP* strip);

/*
    Bank
*/
FMOD_RESULT F_API FMOD_Studio_Bank_Unload(FMOD_STUDIO_BANK* bank);
FMOD_RESULT F_API FMOD_Studio_Bank_LoadSampleData(FMOD_STUDIO_BANK* bank);
FMOD_RESULT F_API FMOD_Studio_Bank_UnloadSampleData(FMOD_STUDIO_BANK* bank);
FMOD_RESULT F_API FMOD_Studio_Bank_GetSampleLoadingState(FMOD_STUDIO_BANK* bank, FMOD_STUDIO_LOADING_STATE* state);


#ifdef __cplusplus
}
#endif

#endif //_FMOD_STUDIO_H
