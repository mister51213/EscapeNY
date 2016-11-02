#pragma once

#include "Actor_Dynamic.h"
#include "Light_Spot.h"

class Actor_Light :
    public Actor_Dynamic
{
public:
    Actor_Light()  = default;
    ~Actor_Light() = default;

    void Initialize(const DirectX::XMFLOAT3 &Position, const DirectX::XMFLOAT3 &LookAt);

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

