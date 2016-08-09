//////////////////////////////////////////////////////////
// The input object will store the state of each key 
// in a keyboard array. When queried it will tell the 
// calling functions if a certain key is pressed.
//////////////////////////////////////////////////////////
#pragma once

#include "Utilities.h"
#include "Camera.h"

#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

class Input
{
public:
    Input(HWND& hWnd);
    Input(const Input&);
    ~Input();

    void SetCam(std::shared_ptr<Camera>& pCam);

    void Initialize();

    void KeyDown(unsigned int);
    void KeyUp(unsigned int);

    bool IsKeyDown(unsigned int);

    void OnMouseDown(WPARAM btnState, int x, int y);
    void OnMouseUp(WPARAM btnState, int x, int y);
    void OnMouseMove(WPARAM btnState, int x, int y);

    XMFLOAT3 m_LastMousePos;

    XMFLOAT3 GetCamOffset()
    {
        return m_camOffset;
    }

    float GetTheta()
    {
        return m_Theta;
    }
    float GetPhi()
    {
        return m_Phi;
    }
    float GetRadius()
    {
        return m_Radius;
    }


private:
    bool m_keys[256];
    HWND m_hMainWnd = nullptr;

    // Camera lookat and movement functions
    XMFLOAT3 m_camOffset = { 0.f,0.f,0.f };
    float m_Theta = 1.5f*XM_PI;
    float m_Phi = XM_PIDIV4;
    float m_Radius = 5.0f;

    std::shared_ptr<Camera> m_pCamera;
};

#endif