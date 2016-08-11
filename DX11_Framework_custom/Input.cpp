#include "Input.h"

//////////////////////////////////////////////////////////
// The input object will store the state of each key 
// in a keyboard array. When queried it will tell the 
// calling functions if a certain key is pressed.
//////////////////////////////////////////////////////////

Input::Input()
{
}

Input::~Input()
{
	// Raw input test
	// Unregister mouse as raw input device
	RAWINPUTDEVICE rawDevice{};
	rawDevice.usUsagePage = 1;
	rawDevice.usUsage = 2;
	rawDevice.dwFlags = RIDEV_REMOVE;
	rawDevice.hwndTarget = nullptr;
	RegisterRawInputDevices( &rawDevice, 1, sizeof( RAWINPUTDEVICE ) );
}

void Input::Initialize( HWND WinHandle )
{
	// Raw input test
	// Register the mouse for raw input
	RAWINPUTDEVICE rawDevice{};
	rawDevice.usUsagePage = 1;
	rawDevice.usUsage = 2;
	rawDevice.dwFlags = RIDEV_NOLEGACY;
	rawDevice.hwndTarget = WinHandle;
	RegisterRawInputDevices( &rawDevice, 1, sizeof( RAWINPUTDEVICE ) );

	int i;
	
	// Initialize all the keys to being released and not pressed.
	for(i=0; i<256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void Input::OnLeftDown( int RelativeX, int RelativeY )
{
	m_leftDown = true;
}

void Input::OnLeftUp( int RelativeX, int RelativeY )
{
	m_leftDown = false;
}

void Input::OnRightDown( int RelativeX, int RelativeY )
{
	m_rightDown = true;
}

void Input::OnRightUp( int RelativeX, int RelativeY )
{
	m_rightDown = false;
}

void Input::OnMouseMove( int RelativeX, int RelativeY )
{
	m_x += RelativeX;
	m_y += RelativeY;
}

void Input::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	return;
}


void Input::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	return;
}

// CODE_CHANGE: Made function const
bool Input::IsKeyDown(unsigned int key)const
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}