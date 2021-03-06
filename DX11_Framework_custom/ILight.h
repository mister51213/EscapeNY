#pragma once

namespace DirectX
{
	struct XMFLOAT4;
}
struct LightBufferType;

class ILight
{
public:
	virtual const DirectX::XMFLOAT4 &GetColor()const = 0;
  	virtual void SetColor( float R, float G, float B ) = 0;

    virtual float GetIntensity()const = 0;
	virtual void SetIntensity( float Value ) = 0;

    virtual const LightBufferType& GetLightBufferType()const = 0;
};

