#pragma once
#include "Actor.h"
#include "Light_Spot.h"

class Actor_Light :
    public Actor
{
public:
    Actor_Light();
    ~Actor_Light();

    void Initialize(const XMFLOAT3 &Position, const XMFLOAT3 &LookAt);

    const ILight* GetLight() const
    {
        return &m_spotLight;
    }
	ILight* GetLight()
	{
		return &m_spotLight;
	}

private:
    Light_Spot m_spotLight;
};

