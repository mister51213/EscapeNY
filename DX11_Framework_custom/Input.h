//////////////////////////////////////////////////////////
// The input object will store the state of each key 
// in a keyboard array. When queried it will tell the 
// calling functions if a certain key is pressed.
//////////////////////////////////////////////////////////
#pragma once

#include "Includes.h"

// TODO: Make input more versatile, needs actual event handling
// Input needs have three events, one for button press, one for button
// release and one for when no events have been received.
class Input
{
public:
	enum class eInputEvent
	{
		Invalid, Pressed, Released
	};

	class Event
	{
	public:
		Event( eInputEvent eEvent, const unsigned char Key )
			:
			m_eEvent( eEvent ),
			m_key( Key )
		{			
		}
		eInputEvent GetEvent()const
		{
			return m_eEvent;
		}
		bool IsRelease()const
		{
			return m_eEvent == eInputEvent::Released;
		}
		bool IsPressed()const
		{
			return m_eEvent == eInputEvent::Pressed;
		}
		bool IsValid()const
		{
			return m_eEvent != eInputEvent::Invalid;
		}
		unsigned char GetKey()const
		{
			return m_key;
		}
	private:
		eInputEvent m_eEvent;
		unsigned char m_key;
	};

	Input() = default;
	~Input();

	void Initialize( HWND WinHandle );

	// Relative data is only sent when a mouse event happens, 
	// FlushRelativeData resets the relative data back to 0.
	// Gets called once a frame before the message loop is 
	// processed.
	void FlushRelativeData();

	// Handles mouse events
	void OnMouseInput( const RAWMOUSE &RawMouseInput );

	// GetX gets mouse X position on screen
	int GetX() const;
	// GetY gets mouse Y position on screen
	int GetY()const;

	// Gets relative X position, not screen coordinates
	int GetRelativeX()const;
	
	// Gets relative Y position, not screen coordinates
	int GetRelativeY()const;

	void KeyDown( unsigned int );
	void KeyUp( unsigned int );

	bool IsKeyDown( unsigned int )const;

	// Will be used for text input later on.  If list is empty
	// both of these functions construct an Event object with
	// an invalid state; eInputEvent::Invalid and keycode is -1;

	// Read returns next event and removes it from the queue
	Event Read();
	// Peek returns next event and doesn't remove from queue
	Event Peek()const;
	//If vector is empty and no mouse buttons pressed, then return false
	bool AnyPressed()const;


private:
	bool m_keys[ 256 ];
	bool m_leftDown = false, m_rightDown = false;

	POINT m_position, m_relPosition;
	RECT m_clamp;

	// Used for the AnyPressed function.  
	std::vector<unsigned int> m_keysPressed;
	std::queue<Event> m_eventQueue;
};
