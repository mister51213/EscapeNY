#pragma once
#include "ILight.h"
class Light_Base :
	public ILight
{
public:
	Light_Base( eLightType Type )
	{
		m_lightBuffer.type = Type;
	}
		
	virtual ~Light_Base() = default;

	const DirectX::XMFLOAT4 &GetColor()const override;
	void SetColor( float R, float G, float B )override;

	float GetIntensity()const override;
	void SetIntensity( float Value ) override;
	const LightBufferType& GetLightBufferType()const override;

protected:
	LightBufferType m_lightBuffer;
};

