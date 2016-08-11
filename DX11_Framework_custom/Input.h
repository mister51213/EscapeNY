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

	void Initialize(HWND WinHandle);

	void OnLeftDown( int RelativeX, int RelativeY );
	void OnLeftUp( int RelativeX, int RelativeY );
	void OnRightDown( int RelativeX, int RelativeY );
	void OnRightUp( int RelativeX, int RelativeY );
	void OnMouseMove( int RelativeX, int RelativeY );


	void KeyDown( unsigned int );
	void KeyUp( unsigned int );

	// CODE_CHANGE: Made function const
	bool IsKeyDown( unsigned int )const;

private:
	bool m_keys[ 256 ];

	bool m_leftDown, m_rightDown;
	int m_x, m_y;
};
