#pragma once
#include "Actor.h"
#include "Light_Spot.h"

class Actor_Light :
    public Actor
{
public:
    Actor_Light();
    ~Actor_Light();

    void Initialize()
    {
        m_spotLight.SetColor(1.f, 1.f, 1.f);
    }

    const ILight* GetLight() const
    {
        return &m_spotLight;
    }

private:
    Light_Spot m_spotLight;
};

