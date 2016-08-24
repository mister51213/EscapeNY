#pragma once
#include "ILightPoint.h"
class LightPoint:
	public ILightPoint
{
public:
	LightPoint();
	~LightPoint();

	const DirectX::XMFLOAT4 &GetAmbient()const override;
	void SetAmbient( float R, float G, float B ) override;

	const DirectX::XMFLOAT4 &GetColor()const override;
	void SetColor( float R, float G, float B ) override;

	float GetIntensity()const override;
	void SetIntensity( float Value ) override;

	void SetPosition( float X, float Y, float Z )override;
	DirectX::XMFLOAT3 GetPosition()const override;

private:
	LightPointBufferType m_buffer;
};

