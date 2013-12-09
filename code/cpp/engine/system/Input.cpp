#include "Input.h"

#if !PLATFORM_MOBILE

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>

#include <system/Window.h>
#include <system/TouchInputManager.h>
#include <system/Timer.h>
#include <common/Debug.h>

// Uncomment this to use controller!
// #define USE_XBOX_CONTROLLER

#if USE_XBOX_CONTROLLER
#include <Windows.h>
#include <XInput.h>
//#pragma comment(lib, "XInput.lib")
#pragma comment(lib, "XINPUT9_1_0.LIB")
#endif

namespace funk
{
	Input::Input()
        : m_iCurrentState(0)
        , m_mousePos(0,0)
        , m_mouseRel(0,0)
        , m_doubleClicked(false)
		, m_padLeftTrigger(0)
		, m_padRightTrigger(0)
		, m_padLeftStick(0.0f, 0.0f)
		, m_padRightStick(0.0f, 0.0f)
	{
		memset(m_padButtonState, 0, sizeof(m_padButtonState));
		BuildMapStringKey();

		Update();
		Update();
		
		// need to enable this to gather unicode text input events..
		SDL_StartTextInput();

		LOG("Input initialized");
	}

	Input::~Input()
	{;}

	void Input::Update()
	{
		// Key state
		const Uint8* keyState = SDL_GetKeyboardState( NULL );
		memcpy( m_arrayKeyStates[m_iCurrentState], keyState, MAX_KEYS);
		memset( &m_key_pressed_data, 0, sizeof(m_key_pressed_data) );

		// Mouse state
		int x, y;
		m_mouseState[m_iCurrentState] = SDL_GetMouseState( &x, &y );
		memset( m_mouseWheelHit, 0, sizeof(m_mouseWheelHit) );
		v2i mousePos;
		mousePos.x = x;
		mousePos.y = Window::Get()->Sizei().y - y;

		m_mouseRel = mousePos - m_mousePos;
		m_mousePos = mousePos;		

#ifdef USE_XBOX_CONTROLLER
		// Pad state
		XINPUT_STATE state = { 0 };
		DWORD result = XInputGetState(0, &state);

		m_padButtonState[m_iCurrentState] = (int)state.Gamepad.wButtons;
		m_padLeftTrigger = state.Gamepad.bLeftTrigger;
		m_padRightTrigger = state.Gamepad.bRightTrigger;
		m_padLeftStick = v2(state.Gamepad.sThumbLX, state.Gamepad.sThumbLY) / 32768.0f;
		m_padRightStick = v2(state.Gamepad.sThumbRX, state.Gamepad.sThumbRY) / 32768.0f;
#endif

		m_iCurrentState ^= 1;

		DetectDoubleClick();
		HandleTouch();
	}
	
	bool Input::IsMouseDownInState( int iState, int mouseKey ) const
	{
		return (m_mouseState[iState] & SDL_BUTTON(mouseKey)) != 0;
	}
	
	void Input::HandleTouch()
	{
		// this function basically transfers mouse touches to 
	
		if ( TouchInputManager::InstExists() )
		{
			const int mouse_btn = 1;
			static TouchInput touch = {0};
			
			v2 touch_pos = v2((float)GetMousePos().x, (float)GetMousePos().y);
			v2 touch_pos_prev = touch_pos + v2((float)GetMousePosRel().x, (float)GetMousePosRel().y);
			touch.pos = touch_pos;
			touch.pos_prev = touch_pos_prev;
		
			if ( DidMouseJustGoDown(mouse_btn) ) // mouse down
			{
				touch.id++;
				touch.tap_count = 0;
				touch.pos_prev = touch.pos; // mouse down doesn't move
				touch.pos_start = touch.pos; // mouse down doesn't move
				touch.state = TOUCHEVENT_BEGIN;
				
				TouchInputManager::Ref().OnEventTouchBegin(touch);
			}
			else if ( DidMouseJustGoUp(mouse_btn) ) // mouse up
			{
				touch.state = TOUCHEVENT_END;
				TouchInputManager::Ref().OnEventTouchEnd(touch);
			}
			else if ( IsMouseDown(mouse_btn) ) // mouse move
			{
				touch.state = TOUCHEVENT_MOVE;
				TouchInputManager::Ref().OnEventTouchMove(touch);
			}
		}
	}

	void Input::ConsumeMouseClick( int btn )
	{
		uint8_t mask =  SDL_BUTTON(btn);

		m_mouseState[m_iCurrentState]	|= mask;
		m_mouseState[m_iCurrentState^1] |= mask;
	}

	void Input::ConsumeKeyPress( uint32_t scan_code )
	{
		m_arrayKeyStates[m_iCurrentState][ scan_code ] = 1;
		m_arrayKeyStates[m_iCurrentState^1][ scan_code ] = 1;
	}

	uint32_t Input::GetCharScanCode( char c ) const
	{
		SDL_Keycode key_code(c);
		SDL_Scancode scan_code = SDL_GetScancodeFromKey( key_code );

		return scan_code;
	}

	void Input::DetectDoubleClick()
	{
		const float DOUBLE_CLICK_THRESHOLD_SECS = 0.35f;
		const float DOUBLE_CLICK_DIST_THRESHOLD = 5.0f;
		static Timer clickTimer;
		static bool tripple_click_buffer_check = false;

		m_doubleClicked = false;
		
		// did you just double click?
		if ( DidMouseJustGoDown(1) && length(m_mouseRel) < DOUBLE_CLICK_DIST_THRESHOLD  )
		{
			float time = clickTimer.GetTimeSecs();
			
			// sometimes, use will accidently tripple-click
			// you do not want to register that as two double-clicks
			if ( tripple_click_buffer_check )
			{
				if ( time < DOUBLE_CLICK_THRESHOLD_SECS )
				{
					tripple_click_buffer_check = false;
					return;
				}
			}
			
			m_doubleClicked = ( time < DOUBLE_CLICK_THRESHOLD_SECS );
			tripple_click_buffer_check = m_doubleClicked; // enable buffer
			clickTimer.Start();
		}
	}

	void Input::BuildMapStringKey()
	{
		m_mapStringKey[HashString("BACKSPACE")]=SDL_SCANCODE_BACKSPACE;
		m_mapStringKey[HashString("TAB")]=SDL_SCANCODE_TAB;
		m_mapStringKey[HashString("CLEAR")]=SDL_SCANCODE_CLEAR;
		m_mapStringKey[HashString("RETURN")]=SDL_SCANCODE_RETURN;
		m_mapStringKey[HashString("PAUSE")]=SDL_SCANCODE_PAUSE;
		m_mapStringKey[HashString("ESCAPE")]=SDL_SCANCODE_ESCAPE;
		m_mapStringKey[HashString("SPACE")]=SDL_SCANCODE_SPACE;
		m_mapStringKey[HashString("EXCLAIM")]=SDL_SCANCODE_KP_EXCLAM;
		m_mapStringKey[HashString("HASH")]=SDL_SCANCODE_NONUSHASH;
		m_mapStringKey[HashString("AMPERSAND")]=SDL_SCANCODE_KP_AMPERSAND;
		m_mapStringKey[HashString("LEFTPAREN")]=SDL_SCANCODE_KP_LEFTPAREN;
		m_mapStringKey[HashString("RIGHTPAREN")]=SDL_SCANCODE_KP_RIGHTPAREN;
		m_mapStringKey[HashString("COMMA")]=SDL_SCANCODE_COMMA;
		m_mapStringKey[HashString("MINUS")]=SDL_SCANCODE_MINUS;
		m_mapStringKey[HashString("PERIOD")]=SDL_SCANCODE_PERIOD;
		m_mapStringKey[HashString("SLASH")]=SDL_SCANCODE_SLASH;
		m_mapStringKey[HashString("0")]=SDL_SCANCODE_0;
		m_mapStringKey[HashString("1")]=SDL_SCANCODE_1;
		m_mapStringKey[HashString("2")]=SDL_SCANCODE_2;
		m_mapStringKey[HashString("3")]=SDL_SCANCODE_3;
		m_mapStringKey[HashString("4")]=SDL_SCANCODE_4;
		m_mapStringKey[HashString("5")]=SDL_SCANCODE_5;
		m_mapStringKey[HashString("6")]=SDL_SCANCODE_6;
		m_mapStringKey[HashString("7")]=SDL_SCANCODE_7;
		m_mapStringKey[HashString("8")]=SDL_SCANCODE_8;
		m_mapStringKey[HashString("9")]=SDL_SCANCODE_9;
		m_mapStringKey[HashString("COLON")]=SDL_SCANCODE_KP_COLON;
		m_mapStringKey[HashString("SEMICOLON")]=SDL_SCANCODE_SEMICOLON;
		m_mapStringKey[HashString("LESS")]=SDL_SCANCODE_KP_LESS;
		m_mapStringKey[HashString("EQUALS")]=SDL_SCANCODE_EQUALS;
		m_mapStringKey[HashString("GREATER")]=SDL_SCANCODE_KP_GREATER;
		m_mapStringKey[HashString("QUESTION")]=63;
		m_mapStringKey[HashString("AT")]=SDL_SCANCODE_KP_AT;
		m_mapStringKey[HashString("LEFTBRACKET")]=SDL_SCANCODE_LEFTBRACKET;
		m_mapStringKey[HashString("BACKSLASH")]=SDL_SCANCODE_BACKSLASH;
		m_mapStringKey[HashString("RIGHTBRACKET")]=SDL_SCANCODE_RIGHTBRACKET;
		m_mapStringKey[HashString("a")]=m_mapStringKey[HashString("A")]=SDL_SCANCODE_A;
		m_mapStringKey[HashString("b")]=m_mapStringKey[HashString("B")]=SDL_SCANCODE_B;
		m_mapStringKey[HashString("c")]=m_mapStringKey[HashString("C")]=SDL_SCANCODE_C;
		m_mapStringKey[HashString("d")]=m_mapStringKey[HashString("D")]=SDL_SCANCODE_D;
		m_mapStringKey[HashString("e")]=m_mapStringKey[HashString("E")]=SDL_SCANCODE_E;
		m_mapStringKey[HashString("f")]=m_mapStringKey[HashString("F")]=SDL_SCANCODE_F;
		m_mapStringKey[HashString("g")]=m_mapStringKey[HashString("G")]=SDL_SCANCODE_G;
		m_mapStringKey[HashString("h")]=m_mapStringKey[HashString("H")]=SDL_SCANCODE_H;
		m_mapStringKey[HashString("i")]=m_mapStringKey[HashString("I")]=SDL_SCANCODE_I;
		m_mapStringKey[HashString("j")]=m_mapStringKey[HashString("J")]=SDL_SCANCODE_J;
		m_mapStringKey[HashString("k")]=m_mapStringKey[HashString("K")]=SDL_SCANCODE_K;
		m_mapStringKey[HashString("l")]=m_mapStringKey[HashString("L")]=SDL_SCANCODE_L;
		m_mapStringKey[HashString("m")]=m_mapStringKey[HashString("M")]=SDL_SCANCODE_M;
		m_mapStringKey[HashString("n")]=m_mapStringKey[HashString("N")]=SDL_SCANCODE_N;
		m_mapStringKey[HashString("o")]=m_mapStringKey[HashString("O")]=SDL_SCANCODE_O;
		m_mapStringKey[HashString("p")]=m_mapStringKey[HashString("P")]=SDL_SCANCODE_P;
		m_mapStringKey[HashString("q")]=m_mapStringKey[HashString("Q")]=SDL_SCANCODE_Q;
		m_mapStringKey[HashString("r")]=m_mapStringKey[HashString("R")]=SDL_SCANCODE_R;
		m_mapStringKey[HashString("s")]=m_mapStringKey[HashString("S")]=SDL_SCANCODE_S;
		m_mapStringKey[HashString("t")]=m_mapStringKey[HashString("T")]=SDL_SCANCODE_T;
		m_mapStringKey[HashString("u")]=m_mapStringKey[HashString("U")]=SDL_SCANCODE_U;
		m_mapStringKey[HashString("v")]=m_mapStringKey[HashString("V")]=SDL_SCANCODE_V;
		m_mapStringKey[HashString("w")]=m_mapStringKey[HashString("W")]=SDL_SCANCODE_W;
		m_mapStringKey[HashString("x")]=m_mapStringKey[HashString("X")]=SDL_SCANCODE_X;
		m_mapStringKey[HashString("y")]=m_mapStringKey[HashString("Y")]=SDL_SCANCODE_Y;
		m_mapStringKey[HashString("z")]=m_mapStringKey[HashString("Z")]=SDL_SCANCODE_Z;
		m_mapStringKey[HashString("DELETE")]=SDL_SCANCODE_DELETE;

		m_mapStringKey[HashString("KP0")]=SDL_SCANCODE_KP_0;
		m_mapStringKey[HashString("KP1")]=SDL_SCANCODE_KP_1;
		m_mapStringKey[HashString("KP2")]=SDL_SCANCODE_KP_2;
		m_mapStringKey[HashString("KP3")]=SDL_SCANCODE_KP_3;
		m_mapStringKey[HashString("KP4")]=SDL_SCANCODE_KP_4;
		m_mapStringKey[HashString("KP5")]=SDL_SCANCODE_KP_5;
		m_mapStringKey[HashString("KP6")]=SDL_SCANCODE_KP_6;
		m_mapStringKey[HashString("KP7")]=SDL_SCANCODE_KP_7;
		m_mapStringKey[HashString("KP8")]=SDL_SCANCODE_KP_8;
		m_mapStringKey[HashString("KP9")]=SDL_SCANCODE_KP_9;
		m_mapStringKey[HashString("KP_PERIOD")]=SDL_SCANCODE_KP_PERIOD;
		m_mapStringKey[HashString("KP_DIVIDE")]=SDL_SCANCODE_KP_DIVIDE;
		m_mapStringKey[HashString("KP_MULTIPLY")]=SDL_SCANCODE_KP_MULTIPLY;
		m_mapStringKey[HashString("KP_MINUS")]=SDL_SCANCODE_KP_MINUS;
		m_mapStringKey[HashString("KP_PLUS")]=SDL_SCANCODE_KP_PLUS;
		m_mapStringKey[HashString("KP_ENTER")]=SDL_SCANCODE_KP_ENTER;
		m_mapStringKey[HashString("KP_EQUALS")]=SDL_SCANCODE_KP_EQUALS;
		m_mapStringKey[HashString("UP")]=SDL_SCANCODE_UP;
		m_mapStringKey[HashString("DOWN")]=SDL_SCANCODE_DOWN;
		m_mapStringKey[HashString("RIGHT")]=SDL_SCANCODE_RIGHT;
		m_mapStringKey[HashString("LEFT")]=SDL_SCANCODE_LEFT;
		m_mapStringKey[HashString("INSERT")]=SDL_SCANCODE_INSERT;
		m_mapStringKey[HashString("HOME")]=SDL_SCANCODE_HOME;
		m_mapStringKey[HashString("END")]=SDL_SCANCODE_END;
		m_mapStringKey[HashString("PAGEUP")]=SDL_SCANCODE_PAGEUP;
		m_mapStringKey[HashString("PAGEDOWN")]=SDL_SCANCODE_PAGEDOWN;
		m_mapStringKey[HashString("F1")]=SDL_SCANCODE_F1;
		m_mapStringKey[HashString("F2")]=SDL_SCANCODE_F2;
		m_mapStringKey[HashString("F3")]=SDL_SCANCODE_F3;
		m_mapStringKey[HashString("F4")]=SDL_SCANCODE_F4;
		m_mapStringKey[HashString("F5")]=SDL_SCANCODE_F5;
		m_mapStringKey[HashString("F6")]=SDL_SCANCODE_F6;
		m_mapStringKey[HashString("F7")]=SDL_SCANCODE_F7;
		m_mapStringKey[HashString("F8")]=SDL_SCANCODE_F8;
		m_mapStringKey[HashString("F9")]=SDL_SCANCODE_F9;
		m_mapStringKey[HashString("F10")]=SDL_SCANCODE_F10;
		m_mapStringKey[HashString("F11")]=SDL_SCANCODE_F11;
		m_mapStringKey[HashString("F12")]=SDL_SCANCODE_F12;
		m_mapStringKey[HashString("F13")]=SDL_SCANCODE_F13;
		m_mapStringKey[HashString("F14")]=SDL_SCANCODE_F14;
		m_mapStringKey[HashString("F15")]=SDL_SCANCODE_F15;
		m_mapStringKey[HashString("NUMLOCK")]=SDL_SCANCODE_NUMLOCKCLEAR;
		m_mapStringKey[HashString("CAPSLOCK")]=SDL_SCANCODE_CAPSLOCK;
		m_mapStringKey[HashString("SCROLLOCK")]=SDL_SCANCODE_SCROLLLOCK;
		m_mapStringKey[HashString("RSHIFT")]=SDL_SCANCODE_RSHIFT;
		m_mapStringKey[HashString("LSHIFT")]=SDL_SCANCODE_LSHIFT;
		m_mapStringKey[HashString("RCTRL")]=SDL_SCANCODE_RCTRL;
		m_mapStringKey[HashString("LCTRL")]=SDL_SCANCODE_LCTRL;
		m_mapStringKey[HashString("RALT")]=SDL_SCANCODE_RALT;
		m_mapStringKey[HashString("LALT")]=SDL_SCANCODE_LALT;
		m_mapStringKey[HashString("MODE")]=SDL_SCANCODE_MODE;
		m_mapStringKey[HashString("APPLICATION")]=SDL_SCANCODE_APPLICATION;
		m_mapStringKey[HashString("HELP")]=SDL_SCANCODE_HELP;
		m_mapStringKey[HashString("PRINTSCREEN")]=SDL_SCANCODE_PRINTSCREEN;
		m_mapStringKey[HashString("SYSREQ")]=SDL_SCANCODE_SYSREQ;
		m_mapStringKey[HashString("MENU")]=SDL_SCANCODE_MENU;
		m_mapStringKey[HashString("POWER")]=SDL_SCANCODE_POWER;
		m_mapStringKey[HashString("UNDO")]=SDL_SCANCODE_UNDO;
		m_mapStringKey[HashString("LGUI")]=SDL_SCANCODE_LGUI;
		
		m_mapStringKey[HashString("AUDIONEXT")]=SDL_SCANCODE_AUDIONEXT;
		m_mapStringKey[HashString("AUDIOPREV")]=SDL_SCANCODE_AUDIOPREV;
		m_mapStringKey[HashString("AUDIOSTOP")]=SDL_SCANCODE_AUDIOSTOP;
		m_mapStringKey[HashString("AUDIOPLAY")]=SDL_SCANCODE_AUDIOPLAY;
		m_mapStringKey[HashString("AUDIOMUTE")]=SDL_SCANCODE_AUDIOMUTE;
		m_mapStringKey[HashString("MEDIASELECT")]=SDL_SCANCODE_MEDIASELECT;
		m_mapStringKey[HashString("VOLUMEUP")]=SDL_SCANCODE_VOLUMEUP;
		m_mapStringKey[HashString("VOLUMEDOWN")]=SDL_SCANCODE_VOLUMEDOWN;

#ifdef USE_XBOX_CONTROLLER
		m_padMapStringKey[HashString("DPAD_UP")] = XINPUT_GAMEPAD_DPAD_UP;
		m_padMapStringKey[HashString("DPAD_DOWN")] = XINPUT_GAMEPAD_DPAD_DOWN;
		m_padMapStringKey[HashString("DPAD_LEFT")] = XINPUT_GAMEPAD_DPAD_LEFT;
		m_padMapStringKey[HashString("DPAD_RIGHT")] = XINPUT_GAMEPAD_DPAD_RIGHT;
		m_padMapStringKey[HashString("START")] = XINPUT_GAMEPAD_START;
		m_padMapStringKey[HashString("BACK")] = XINPUT_GAMEPAD_BACK;
		m_padMapStringKey[HashString("LEFT_THUMB")] = XINPUT_GAMEPAD_LEFT_THUMB;
		m_padMapStringKey[HashString("RIGHT_THUMB")] = XINPUT_GAMEPAD_RIGHT_THUMB;
		m_padMapStringKey[HashString("LEFT_SHOULDER")] = XINPUT_GAMEPAD_LEFT_SHOULDER;
		m_padMapStringKey[HashString("RIGHT_SHOULDER")] = XINPUT_GAMEPAD_RIGHT_SHOULDER;
		m_padMapStringKey[HashString("A")] = XINPUT_GAMEPAD_A;
		m_padMapStringKey[HashString("B")] = XINPUT_GAMEPAD_B;
		m_padMapStringKey[HashString("X")] = XINPUT_GAMEPAD_X;
		m_padMapStringKey[HashString("Y")] = XINPUT_GAMEPAD_Y;
#endif
	}

	int Input::ShowMouseCursor(bool show)
	{
		return SDL_ShowCursor(show?1:0);
	}

	void Input::SetMousePos( int x, int y )
	{
		SDL_Window* window = SDL_GetKeyboardFocus();
		SDL_WarpMouseInWindow(  window, x, Window::Get()->Sizei().y - y );
		m_mousePos = v2i(x,y);
	}

	void Input::HandleEvent( SDL_Event *event )
	{
		if ( event->type == SDL_KEYDOWN )
		{
			/* Now uses SDL_TEXTINPUT
			SDL_Keycode key_code = event->key.keysym.sym;
			if ( key_code < SDLK_SCANCODE_MASK )
			{
				//m_key_pressed = key_code;
			}
			*/
		}
		else if ( event->type == SDL_KEYUP )
		{
			/*	Now uses SDL_TEXTINPUT
			SDL_Keycode key_code = event->key.keysym.sym;
			if ( key_code < SDLK_SCANCODE_MASK )
			{
				m_keyUp = key_code;
			}
			*/
		}
		else if ( event->type == SDL_TEXTINPUT )
		{
			m_key_pressed_data.character = event->text.text[0];
			//TRACE("text: '%s'\n", event->text.text  );
		}
		else if ( event->type == SDL_MOUSEBUTTONDOWN  )
		{
			
		}
		else if ( event->type == SDL_MOUSEWHEEL )
		{
			if (event->wheel.y < 0 ) m_mouseWheelHit[MOUSEWHEEL_UP] = true;
			else if (event->wheel.y > 0) m_mouseWheelHit[MOUSEWHEEL_DOWN] = true;
		}
	}
}

#endif // PLATFORM_MOBILE