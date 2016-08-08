//////////////////////////////////////////////////////////
// The input object will store the state of each key 
// in a keyboard array. When queried it will tell the 
// calling functions if a certain key is pressed.
//////////////////////////////////////////////////////////
#pragma once

#include "Utilities.h"

#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

class Input
{
public:
    Input(HWND& hWnd);
	Input(const Input&);
	~Input();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

    void OnMouseDown(WPARAM btnState, int x, int y);
    void OnMouseUp(WPARAM btnState, int x, int y);
    void OnMouseMove(WPARAM btnState, int x, int y);

    XMFLOAT3 m_LastMousePos;

private:
	bool m_keys[256];
    HWND m_hMainWnd = nullptr;

    // USED FOR MOUSE ROTATION FUNCTIONS from Luna
    float m_Theta = 1.5f*XM_PI;
    float m_Phi = XM_PIDIV4;
    float m_Radius = 5.0f;
};

#endif