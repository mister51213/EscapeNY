//////////////////////////////////////////////////////////
// The input object will store the state of each key 
// in a keyboard array. When queried it will tell the 
// calling functions if a certain key is pressed.
//////////////////////////////////////////////////////////

#include "Input.h"


Input::Input(HWND& hMainWnd):
    m_hMainWnd(hMainWnd),
    m_LastMousePos({ 0.f,0.f,0.f })
{}

Input::Input(const Input& other)
{}

Input::~Input()
{}

void Input::Initialize()
{
	int i;
	
	// Initialize all the keys to being released and not pressed.
	for(i=0; i<256; i++)
	{
		m_keys[i] = false;
	}

	// NOTE: You aren't doing anything with x,y,z; so why is this here?
    // (Is this needed?) Convert Spherical to Cartesian coordinates.
    float x = m_Radius*sinf(m_Phi)*cosf(m_Theta);
    float z = m_Radius*sinf(m_Phi)*sinf(m_Theta);
    float y = m_Radius*cosf(m_Phi);
	// NOTE: This looks like it was a copy/paste of Luna's code, we don't have a ShapeBuilder class
    // NOTE: This info is passed on to the camera POSITION in ShapeBuilder.cpp line 171~

	return;
}

#pragma region ***********MOUSE INPUT***********
//  WINDOWS CONVENTIONS - 
// If a message takes a pointer, the pointer is usually passed in the LPARAM. 
// If the message takes a handle or an integer, then it is passed in the WPARAM. 

// NOTE: You don't use the WPARAM, why even pass it?
void Input::OnMouseDown(WPARAM btnState, int x, int y)
{
    m_LastMousePos.x = x;
    m_LastMousePos.y = y;

    SetCapture(m_hMainWnd);
}

// NOTE: You don't use the WPARAM, why even pass it?
void Input::OnMouseUp(WPARAM btnState, int x, int y)
{
    ReleaseCapture();
}

void Input::OnMouseMove(WPARAM btnState, int x, int y)
{
	// NOTE: Function is specific to camera rotation, should be moved to 
	//		 Camera::GetInput.  OnMouseMove, should update the position
	//		 of the mouse.  The OnMouseDown and OnMouseUp should hold
	//		 the button states.  The Set/Release capture should be set
	//		 when mouse enters or extis the window...
	//		 For game purposes, the mouse should probably be clamped to window.  

	// LEFT BUTTON - rotate the object
    if((btnState & MK_LBUTTON) != 0)
    {
        // Make each pixel correspond to a quarter of a degree.
        float dx = XMConvertToRadians(0.25f*static_cast<float>(x - m_LastMousePos.x));
        float dy = XMConvertToRadians(0.25f*static_cast<float>(y - m_LastMousePos.y));

        // Update angles based on input to orbit camera around box.
        m_Theta += dx;
        m_Phi += dy;

        // Restrict the angle mPhi. 
        m_Phi = Clamp(m_Phi, 0.1f, PI - 0.1f);
    }
    // RIGHT BUTTON - zoom in and out
    else if((btnState & MK_RBUTTON) != 0)
    {
        // Make each pixel correspond to 0.005 unit in the scene.
        float dx = 0.005f*static_cast<float>(x - m_LastMousePos.x);
        float dy = 0.005f*static_cast<float>(y - m_LastMousePos.y);

        // Update the camera radius based on input.
        m_Radius += dx - dy;

        // Restrict the radius.
        m_Radius = Clamp(m_Radius, 3.0f, 15.0f);
    }

    // TODO: Comment explaining this
    m_LastMousePos.x = x;
    m_LastMousePos.y = y;
}

#pragma endregion

// KEYBOARD INPUT
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

bool Input::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}
