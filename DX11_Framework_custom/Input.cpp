#include "Input.h"

//////////////////////////////////////////////////////////
// The input object will store the state of each key 
// in a keyboard array. When queried it will tell the 
// calling functions if a certain key is pressed.
//////////////////////////////////////////////////////////

Input::~Input()
{
	// Raw input test
	// Unregister mouse as raw input device
	RAWINPUTDEVICE rawDevice{};
	rawDevice.usUsagePage = 1;
	rawDevice.usUsage = 2;
	rawDevice.dwFlags = RIDEV_REMOVE;
	
	// Setting hwndTarget to null unregisters the device
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

	ZeroMemory( m_keys, 256 );
	FlushRelativeData();

	GetWindowRect( WinHandle, &m_clamp );
	m_position.x = ( m_clamp.right - m_clamp.left ) / 2;
	m_position.y = ( m_clamp.bottom - m_clamp.top ) / 2;
	SetCursorPos( m_position.x, m_position.y );
}

void Input::FlushRelativeData()
{
	m_relPosition = POINT{};
	while ( !m_eventQueue.empty() )
	{
		m_eventQueue.pop();
	}
}

void Input::OnMouseInput( const RAWMOUSE & RawMouseInput )
{
	// Store any relative mouse movements from last frame
	m_relPosition.x = RawMouseInput.lLastX;
	m_relPosition.y = RawMouseInput.lLastY;

	m_leftDown = RawMouseInput.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN;
	m_rightDown = ( ( RawMouseInput.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN ) >> 2 );

	// Clip the cursor to window boundaries
	ClipCursor( &m_clamp );

	// Update Input's mouse position	
	GetCursorPos( &m_position );		

	// Register the new position with Windows
	SetCursorPos( m_position.x, m_position.y );
}

int Input::GetX() const
{
	return m_position.x;
}

int Input::GetY() const
{
	return m_position.y;
}

int Input::GetRelativeX() const
{
	return m_relPosition.x;
}

int Input::GetRelativeY() const
{
	return m_relPosition.y;
}

void Input::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	// Skip adding to queue if auto-repeat...button held too long
	
	m_eventQueue.push( Event( eInputEvent::Pressed, input ) );
	m_keysPressed.push_back( input );
	m_keys[input] = true;
	return;
}


void Input::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;

	// Remove key from the list of keys that are pressed.
	std::vector<unsigned int>::iterator it = 
		std::find( m_keysPressed.begin(), m_keysPressed.end(), input );
	if ( it != m_keysPressed.end() )
	{
		m_keysPressed.erase( it );
	}
	return;
}

bool Input::IsKeyDown(unsigned int key)const
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}


class DoLast
{
public:
	DoLast( std::queue< Input::Event > &Events ) : m_events( Events ) {}
	~DoLast() { m_events.pop(); }
private:
	std::queue< Input::Event > &m_events;
};

Input::Event Input::Read()
{
	if ( m_eventQueue.empty() )
	{
		m_eventQueue.emplace( eInputEvent::Invalid, -1 );
	}

	DoLast doLast( m_eventQueue );
	return m_eventQueue.front();
}

Input::Event Input::Peek()const
{
	if ( m_eventQueue.empty() )
	{
		Input::Event( eInputEvent::Invalid, -1 );
	}

	return m_eventQueue.front();
}

bool Input::AnyPressed() const
{	
	return ( !m_keysPressed.empty() ) | m_leftDown | m_rightDown;
}
