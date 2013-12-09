/*
    fmod_studio.hpp - FMOD Studio API (Draft 2)
    Copyright (c), Firelight Technologies Pty, Ltd. 2012.

    This header is the base header for all other FMOD Studio headers. If you are programming in C use fmod_studio.h
*/

// This is considered confidential information. Please do not distribute!

#ifndef FMOD_STUDIO_HPP
#define FMOD_STUDIO_HPP

#include "fmod_studio_common.h"
#include "fmod_studio.h"

#include "fmod.hpp"

namespace FMOD
{

namespace Studio
{
    typedef FMOD_GUID ID;

    class System;

    class Bank;

    class MixerStrip;
    class EventDescription;

    class EventInstance;
    class ParameterInstance;
    class CueInstance;

    template <class T_Element>
    class Iterator
    {
    public:
        class Impl;
        F_API Iterator();
        F_API Iterator(Impl*);
        F_API ~Iterator();

        // Element access
        bool F_API isValid() const;                                   // True if the iterator is pointing to a valid element.
        FMOD_RESULT F_API currentElement(T_Element* element) const;   // The element the iterator is currently pointing to.

        // Traversal
        FMOD_RESULT F_API advance();                                  // Advance to the next element.

    private:
        Iterator(const Iterator&) { } // disabled
        void operator==(const Iterator&) { } // disabled

        Impl* mImpl;
    };

    typedef Iterator<EventInstance> EventInstanceIterator;

    // Note: These classes use the pointer to implementation idiom, so the usage would look like:
    //
    // System system;
    // ERRCHECK(FMOD::Studio::createSystem(&system));
    // ERRCHECK(system.initialize());
    //
    // EventDescription event;
    // ERRCHECK(system.getEvent(id, FMOD_STUDIO_LOAD_BEGIN_NOW, &event));
    // ERRCHECK(event.playOneshot());

    template <class T_Derived> class HandleFactory;
    template <class T_Derived> class ImplBase;

    template <class T_Derived>
    class Handle
    {
    public:
        // Handle validity
        bool F_API isValid() const;

        class Impl;

    protected:
        F_API Handle();
        F_API Handle(const Handle& other);
        void F_API operator=(const Handle& other);
        F_API ~Handle();

        void setImpl(Impl* impl);
        Impl* impl() const { return mImpl; }

    private:
        friend class HandleFactory<T_Derived>;
        friend class ImplBase<T_Derived>;
        Impl* mImpl;
        void* mData[4];
    };

    FMOD_RESULT F_API parseID(const char* idString, ID* id);

    class System : public Handle<System>
    {
    public:
        static FMOD_RESULT F_API create(System* system, unsigned int headerVersion = FMOD_VERSION);                                                            // Creates a system
        FMOD_RESULT F_API initialize(int maxchannels, FMOD_STUDIO_INITFLAGS studioFlags, FMOD_INITFLAGS flags, void *extradriverdata); // Initializes the system
        FMOD_RESULT F_API release();

        // Playback processing
        FMOD_RESULT F_API update();

        // Low-level API access
        FMOD_RESULT F_API getLowLevelSystem(FMOD::System** system) const;

        // Asset retrieval
        // These functions load data from registered banks if necessary, unless mode is set to FMOD_STUDIO_LOAD_PROHIBITED.
        FMOD_RESULT F_API getEvent(const ID* id, FMOD_STUDIO_LOADING_MODE mode, EventDescription* event) const;
        FMOD_RESULT F_API getMixerStrip(const ID* id, FMOD_STUDIO_LOADING_MODE mode, MixerStrip* mixerStrip) const;
        FMOD_RESULT F_API getBank(const ID* id, Bank* bank) const;

        // Path lookup
        FMOD_RESULT F_API lookupEventID(const char* path, ID* id) const;
        FMOD_RESULT F_API lookupBusID(const char* path, ID* id) const;

        // Listener control
        FMOD_RESULT F_API getListenerAttributes(FMOD_3D_ATTRIBUTES* attributes) const;
        FMOD_RESULT F_API setListenerAttributes(const FMOD_3D_ATTRIBUTES* attributes);

        FMOD_RESULT F_API loadBank(const char* filename, Bank* bank);                                       // Load a bank file in full. Wraps bank registration and loading.

        FMOD_RESULT F_API registerPlugin(const FMOD_DSP_DESCRIPTION* description);
        FMOD_RESULT F_API unregisterPlugin(const char* name);

        FMOD_RESULT F_API unloadAll();
    };

    class EventDescription : public Handle<EventDescription>
    {
    public:
        // Property access
        FMOD_RESULT F_API getID(ID* id) const;
        FMOD_RESULT F_API getUserPropertyCount(int* count) const;
        FMOD_RESULT F_API getUserPropertyByIndex(int index, FMOD_STUDIO_USER_PROPERTY* property) const;       // Retrieve a property created by the user within FMOD Studio.
        FMOD_RESULT F_API getUserProperty(const char* name, FMOD_STUDIO_USER_PROPERTY* property) const;       // Retrieve a property created by the user within FMOD Studio.
        FMOD_RESULT F_API getLength(int* length) const;                                                       // Length in milliseconds
        FMOD_RESULT F_API getMaximumDistance(float* distance) const;

        FMOD_RESULT F_API isOneshot(bool* oneshot) const;
        FMOD_RESULT F_API is3D(bool* is3D) const;                                                             // True if the event has a 3D panner or other 3D-dependent features

        // Playback control
        FMOD_RESULT F_API createInstance(EventInstance* instance) const;                                      // Allocates a new instance of this event.
        FMOD_RESULT F_API enumeratePersistentInstances(EventInstanceIterator* iterator) const;                // Enumerates instances created with createInstance, ignoring those created by playOneshot and
                                                                                                        // those where deferredRelease has been called.

        // Convenience functions
        FMOD_RESULT F_API releaseAllInstances();                                                            // Stops and releases all instances of this event. Invalidates all instance handles.

        // Callbacks
        FMOD_RESULT F_API setCallback(FMOD_STUDIO_EVENT_CALLBACK callback);                                 // The callback set here is copied to new instances at creation time.
        FMOD_RESULT F_API getUserData(void** userData) const;
        FMOD_RESULT F_API setUserData(void* userData);                                                      // The user data set here is copied to new instances at creation time.
    };

    class EventInstance : public Handle<EventInstance>
    {
    public:
        // Property access
        FMOD_RESULT F_API getDescription(EventDescription* description) const;

        // Playback control
        FMOD_RESULT F_API getVolume(float* volume) const;
        FMOD_RESULT F_API setVolume(float volume);

        FMOD_RESULT F_API getPitch(float* pitch) const;
        FMOD_RESULT F_API setPitch(float pitch);

        FMOD_RESULT F_API get3DAttributes(FMOD_3D_ATTRIBUTES* attributes) const;
        FMOD_RESULT F_API set3DAttributes(const FMOD_3D_ATTRIBUTES* attributes);

        FMOD_RESULT F_API getPaused(bool* paused) const;
        FMOD_RESULT F_API setPaused(bool paused);

        FMOD_RESULT F_API start();
        FMOD_RESULT F_API stop(FMOD_STUDIO_STOP_MODE mode);

        FMOD_RESULT F_API getTimelinePosition(int* position) const;
        FMOD_RESULT F_API setTimelinePosition(int position);

        FMOD_RESULT F_API getPlaybackState(FMOD_STUDIO_PLAYBACK_STATE* state) const;

        FMOD_RESULT F_API getChannelGroup(ChannelGroup** group) const;

        FMOD_RESULT F_API release();                                                                // Releases the user reference to this instance and invalidates the instance handle.
                                                                                                    // Instance data is cleaned up when the instance stops (or immediately if the instance is already stopped).
                                                                                                    // Returns an error if the instance is playing and won't stop by itself (since it would never be cleaned up).

        FMOD_RESULT F_API isVirtual(bool* virtualState) const;                                      // Virtual state will be true if a max polyphony setting is preventing this event from being audible.

        FMOD_RESULT F_API getParameter(const char* name, ParameterInstance* parameter) const;
        FMOD_RESULT F_API getParameterByIndex(int index, ParameterInstance* parameter) const;
        FMOD_RESULT F_API getParameterCount(int* parameterCount) const;

        FMOD_RESULT F_API getCue(const char* name, CueInstance* cue) const;
        FMOD_RESULT F_API getCueByIndex(int index, CueInstance* cue) const;
        FMOD_RESULT F_API getCueCount(int* cueCount) const;

        FMOD_RESULT F_API createSubEvent(const char* name, EventInstance* instance) const;

        // Loading control
        FMOD_RESULT F_API getLoadingState(FMOD_STUDIO_LOADING_STATE* state) const;

        // Callbacks
        FMOD_RESULT F_API setCallback(FMOD_STUDIO_EVENT_CALLBACK callback);

        FMOD_RESULT F_API getUserData(void** userData) const;
        FMOD_RESULT F_API setUserData(void* userData);
    };

    class CueInstance : public Handle<CueInstance>
    {
    public:
        FMOD_RESULT F_API trigger();
    };

    class ParameterInstance : public Handle<ParameterInstance>
    {
    public:
        // Playback control
        FMOD_RESULT F_API getValue(float* value) const;
        FMOD_RESULT F_API setValue(float value);
    };

    class MixerStrip : public Handle<MixerStrip>
    {
    public:
        // Playback control
        FMOD_RESULT F_API getFaderLevel(float* level) const;
        FMOD_RESULT F_API setFaderLevel(float level);                                   // Fader level is relative to the fader level set in FMOD Studio.

        FMOD_RESULT F_API getPaused(bool* paused) const;
        FMOD_RESULT F_API setPaused(bool paused);                                     // If paused is true, it overrides the pause state of all objects controlled by this mixer strip. If paused is false, controlled
                                                                                      // objects obey their own pause state.

        FMOD_RESULT F_API stopAllEvents(FMOD_STUDIO_STOP_MODE mode);                  // Stops all events controlled by this mixer strip.
        
        // Low-level API access
        FMOD_RESULT F_API getChannelGroup(FMOD::ChannelGroup** channelgroup) const;

        // Loading control
        FMOD_RESULT F_API getLoadingState(FMOD_STUDIO_LOADING_STATE* state) const;
        FMOD_RESULT F_API release();                                                  // Releases the user reference to this object, allowing the system to clean it up if no internal dependency references exist.
                                                                                      // All handles to this object are invalidated. Cancels any pending user-triggered loads of this object.
    };

    class Bank : public Handle<Bank>
    {
    public:
        // Property access
        FMOD_RESULT F_API getID(ID* id) const;

        // Loading control
        FMOD_RESULT F_API unload();                                               // Unload all assets in this bank. Destroys any instances and invalidates asset handles.
        FMOD_RESULT F_API loadSampleData();                                       // Load all sample data in this bank (non-blocking).
        FMOD_RESULT F_API unloadSampleData();                                     // Unload all sample data in this bank (non-blocking).
        FMOD_RESULT F_API getSampleLoadingState(FMOD_STUDIO_LOADING_STATE* state) const;
    };

} // namespace Studio

} // namespace FMOD

#endif //FMOD_STUDIO_HPP
