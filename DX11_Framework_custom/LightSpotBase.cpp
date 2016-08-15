#include "LightSpotBase.h"



LightSpotBase::LightSpotBase()
{
}


LightSpotBase::~LightSpotBase()
{
}

const DirectX::XMFLOAT3 & LightSpotBase::GetDirection() const
{
	return m_direction;
}

void LightSpotBase::SetDirection( float X, float Y, float Z )
{
	m_direction = { X, Y, Z };
}

const DirectX::XMFLOAT3 & LightSpotBase::GetPosition() const
{
	return m_position;
}

void LightSpotBase::SetPosition( float X, float Y, float Z )
{
	m_position = { X, Y, Z };
}

const DirectX::XMFLOAT4 & LightSpotBase::GetColor() const
{
	return m_color;
}

float LightSpotBase::GetIntensity() const
{
	return m_intensity;
}

void LightSpotBase::SetColor( float R, float G, float B )
{
	m_color = { R,G,B,1.f };	
}

void LightSpotBase::SetIntensity( float Intensity )
{
	m_intensity = Intensity;
}
