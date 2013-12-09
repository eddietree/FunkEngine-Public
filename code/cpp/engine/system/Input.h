#pragma once

#include <unordered_map>
#include <stdint.h>

#include <system/Config.h>
#include <math/v2.h>
#include <math/v2i.h>
#include <common/Singleton.h>
#include <common/Util.h>

// fwd decl
union SDL_Event;

namespace funk
{
#if !PLATFORM_MOBILE
	// non-mobile (PC, MacOSX) uses SDL keyboard/mouse
	
	class Input : public Singleton<Input>
	{
	public:
		static const char KEY_BACKSPACE = '\b';
		static const char KEY_RETURN = '\r';
		static const int  MOUSEWHEEL_UP = 0;
		static const int  MOUSEWHEEL_DOWN = 1;

		void Update();
		void HandleEvent(SDL_Event *event);

		// Mouse
		int ShowMouseCursor(bool show); // returns prev state of cursor (1=displayed before)
		inline bool IsMouseDown( int btn ) const;
		inline bool DidMouseJustGoDown( int btn ) const;
		inline bool DidMouseJustGoUp( int btn ) const;
		void		SetMousePos( int x, int y );
		v2i			GetMousePos() const { return m_mousePos; }
		v2i			GetMousePosRel() const { return m_mouseRel; }
		inline bool DidMouseWheelHit( int btn ) const;
		inline bool DidMouseDoubleClick() const { return m_doubleClicked; }

		// Key
		inline bool IsKeyDown( const char* key ) const;
		inline bool DidKeyJustGoDown( const char* key ) const;
		inline bool DidKeyJustGoUp( const char* key ) const;
		inline char GetKeyPressed() const { return m_key_pressed_data.character; }
		
		// consume events so it doesn't reverb downwards
		void ConsumeMouseClick( int btn );
		void ConsumeKeyPress( uint32_t scan_code );
		inline uint32_t GetScanCode( const char * key ) const;
		uint32_t GetCharScanCode( char c ) const; // converts a key to the uint32_t keyhash for m_mapStringKey

		// Pad
		inline bool IsPadDown(const char* key) const;
		inline bool IsPadPress(const char* key) const;
		inline bool IsPadRelease(const char* key) const;
		inline unsigned char GetPadLeftTrigger() const { return m_padLeftTrigger; }
		inline unsigned char GetPadRightTrigger() const { return m_padRightTrigger; }
		funk::v2 GetPadLeftStick() const { return m_padLeftStick; }
		funk::v2 GetPadRightStick() const { return m_padRightStick; }

	private:
		friend Singleton<Input>;
		Input();
		~Input();

		void BuildMapStringKey();
		inline bool IsKeyDownInState( int iState, unsigned int key ) const;
		bool IsMouseDownInState( int iState, int mouseKey ) const;
		inline bool IsPadDownInState(int iState, unsigned int key) const;
		
		// to help communicate with iTouch devices..
		void HandleTouch();

		void DetectDoubleClick();
		bool m_doubleClicked;

		// Key
		static const int MAX_KEYS = 323;
		uint8_t  m_arrayKeyStates[ 2 ][MAX_KEYS];

		// maps hash(string)=>Scancode
		std::unordered_map< uint32_t, uint32_t > m_mapStringKey;

		struct KeyPressedData
		{
			char character; // human readable
		} m_key_pressed_data;
		

		int m_iCurrentState;

		// mouse
		bool  m_mouseWheelHit[2];
		uint8_t m_mouseState[2];
		v2i m_mousePos;
		v2i m_mouseRel;

		// pad
		std::unordered_map< uint32_t, unsigned int > m_padMapStringKey;
		int m_padButtonState[2];
		unsigned char m_padLeftTrigger;
		unsigned char m_padRightTrigger;
		funk::v2 m_padLeftStick;
		funk::v2 m_padRightStick;
	};

#include "Input.inl"

#else

	// MOBILE DEVICES should just return false
	class Input : public Singleton<Input>
	{
	public:
		static const char KEY_BACKSPACE = 8;
		static const char KEY_RETURN = 13;
		static const int  MOUSEWHEEL_UP = 0;
		static const int  MOUSEWHEEL_DOWN = 1;

		void Update(){;}
		
		// Mouse
		int ShowMouseCursor(bool show){ return 0;}
		inline bool IsMouseDown( int btn ) const { return false;}
		inline bool DidMouseJustGoDown( int btn ) const { return false;}
		inline bool DidMouseJustGoUp( int btn ) const { return false;}
		void		SetMousePos( int x, int y ){;}
		v2i			GetMousePos() const { return v2i(0); }
		v2i			GetMousePosRel() const { return v2i(0); }
		inline bool DidMouseWheelHit( int btn ) const { return false;}
		inline bool DidMouseDoubleClick() const { return false; }
		
		// Key
		inline bool IsKeyDown( const char* key ) const { return false;}
		inline bool DidKeyJustGoDown( const char* key ) const { return false;}
		inline bool DidKeyJustGoUp( const char* key ) const { return false;}
		inline char GetKeyPressed() const { return 0; }
		
		// Pad
		inline bool IsPadDown(const char* key) const { return false;}
		inline bool IsPadPress(const char* key) const { return false;}
		inline bool IsPadRelease(const char* key) const { return false;}
		inline unsigned char GetPadLeftTrigger() const { return 0; }
		inline unsigned char GetPadRightTrigger() const { return 0; }
		funk::v2 GetPadLeftStick() const { return v2(0.0f); }
		funk::v2 GetPadRightStick() const { return v2(0.0f); }

		// consume events so it doesn't reverb downwards
		inline void ConsumeMouseClick( int btn ) {;}
		inline void ConsumeKeyPress( uint32_t key_hashed ) {;}
		
	private:
		friend Singleton<Input>;
		Input(){;}
		~Input() {;}
	};
	
#endif // PLATFORM_MOBILE
} // namespace funk
