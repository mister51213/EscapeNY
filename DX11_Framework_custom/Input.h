//////////////////////////////////////////////////////////
// The input object will store the state of each key 
// in a keyboard array. When queried it will tell the 
// calling functions if a certain key is pressed.
//////////////////////////////////////////////////////////
#pragma once

class Input
{
public:
	Input();
	~Input();

	void Initialize();

	void KeyDown( unsigned int );
	void KeyUp( unsigned int );

	// CODE_CHANGE: Made function const
	bool IsKeyDown( unsigned int )const;

private:
	bool m_keys[ 256 ];
};
