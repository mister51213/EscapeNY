//////////////////////////////////////////////////////////
// The input object will store the state of each key 
// in a keyboard array. When queried it will tell the 
// calling functions if a certain key is pressed.
//////////////////////////////////////////////////////////
#pragma once

#include "Includes.h"

class Input
{
public:
	Input();
	~Input();

	void Initialize( HWND WinHandle );

	// Relative data is only sent when a mouse event happens, 
	// so there is nothing to tell it that the mouse hasn't moved.
	void FlushRelativeData();
	void OnLeftDown( int RelativeX, int RelativeY );
	void OnLeftUp( int RelativeX, int RelativeY );
	void OnRightDown( int RelativeX, int RelativeY );
	void OnRightUp( int RelativeX, int RelativeY );
	void OnMouseMove( int RelativeX, int RelativeY );

	// Mouse will have to have two modes, one for clicking on screen items
	// the other for camera/player movement.

	int GetX() const;

	int GetY()const;

	// Gets relative X position, not screen coordinates
	int GetRelativeX()const;
	
	// Gets relative Y position, not screen coordinates
	int GetRelativeY()const;

	void KeyDown( unsigned int );
	void KeyUp( unsigned int );

	// CODE_CHANGE: Made function const
	bool IsKeyDown( unsigned int )const;

private:
	bool m_keys[ 256 ];

	bool m_leftDown, m_rightDown;
	int m_x, m_y;
	int m_relX, m_relY;
	RECT m_clamp;
};
