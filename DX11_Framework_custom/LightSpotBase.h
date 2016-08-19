#pragma once

#include "ILightSpot.h"


struct SpotLightBuffer
{
	DirectX::XMFLOAT4 m_colorAndIntensity;
	DirectX::XMFLOAT3 m_position;
	float m_innerCone;
	DirectX::XMFLOAT3 m_direction;
	float m_outerCone;
};

class alignas( 16 )LightSpotBase:
	public ILightSpot
{
public:
	LightSpotBase() = default;
	~LightSpotBase() = default;

	// Inherited from ILightDirection
	const DirectX::XMFLOAT3 &GetDirection()const override;
	void SetDirection( float X, float Y, float Z )override;

	// Inherited from ILightPoint
	const DirectX::XMFLOAT3 &GetPosition()const override;
	void SetPosition( float X, float Y, float Z )override;

	// Inherited from ILight
	const DirectX::XMFLOAT4 &GetColor()const override;
	float GetIntensity()const override;
	// Color stores it's color values in RGB, the intensity is stored in the
	// Alpha channel
	void SetColor( float R, float G, float B )override;
	void SetIntensity( float Intensity )override;
	
	// Inherited from ILightSpot
	void SetInnerConeAngle( float InnerAngle )override;
	void SetOuterConeAngle( float OuterAndle )override;
	float GetInnerConeAngle()const override;
	float GetOuterConeAngle()const override;

	const SpotLightBuffer *GetDataBuffer()const;
protected:
	SpotLightBuffer m_buffer;
};

