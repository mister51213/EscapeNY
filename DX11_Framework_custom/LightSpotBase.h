#pragma once
#include "ILightSpot.h"
class LightSpotBase:
	public ILightSpot
{
public:
	LightSpotBase();
	~LightSpotBase();

	// Inherited from ILightDirection
	const DirectX::XMFLOAT3 &GetDirection()const override;
	void SetDirection( float X, float Y, float Z )override;

	// Inherited from ILightPoint
	const DirectX::XMFLOAT3 &GetPosition()const override;
	void SetPosition( float X, float Y, float Z )override;

	// Inherited from ILight
	const DirectX::XMFLOAT4 &GetColor()const override;
	float GetIntensity()const override;
	void SetColor( float R, float G, float B )override;
	void SetIntensity( float Intensity )override;
	
protected:
	DirectX::XMFLOAT4 m_color;
	DirectX::XMFLOAT3 m_position, m_direction;
	float m_intensity;
};

