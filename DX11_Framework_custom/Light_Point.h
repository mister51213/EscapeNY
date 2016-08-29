#pragma once

#include "Light_Base.h"
#include "ILightPoint.h"

class Light_Point :
	public Light_Base,
	public ILightPoint
{
public:

	Light_Point()
		:
		Light_Base( eLightType::POINT_LIGHT )
	{
	}

	~Light_Point() = default;

	const XMFLOAT3& GetPosition() const override
	{
		return m_lightBuffer.lightPosition;
	}
	void SetPosition( const XMFLOAT3& position ) override
	{
		m_lightBuffer.lightPosition = position;
	}

private:
	LightBufferType m_lightBuffer;
};

