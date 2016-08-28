#pragma once
#include "Actor.h"
#include "Light_Spot.h"
#include "Camera.h"

class Actor_Light :
    public Actor, public Camera
{
public:
    Actor_Light();
    void GetKeyInput_Light(const Input & pInput);
    void GetMouseInput_Light(const Input & pInput);
    ~Actor_Light();

    // NOTE: MUST CALL PARENT INITIALIZE FIRST!
    void InitSpotLight()
    {
        m_spotLight.SetDirection(m_Orientation);
        m_spotLight.SetPosition(m_Position);
        m_spotLight.SetColor(1.0f, 1.0f, 1.0f);
    }

    const ILight* GetLight() const
    {
        return &m_spotLight;
    }

protected:
    Light_Spot m_spotLight;
};

