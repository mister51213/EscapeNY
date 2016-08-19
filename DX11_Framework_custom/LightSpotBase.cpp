#include "LightSpotBase.h"



const DirectX::XMFLOAT3 & LightSpotBase::GetDirection() const
{
	return m_buffer.m_direction;
}

void LightSpotBase::SetDirection( float X, float Y, float Z )
{
	m_buffer.m_direction = { X, Y, Z };
}

const DirectX::XMFLOAT3 & LightSpotBase::GetPosition() const
{
	return m_buffer.m_position;
}

void LightSpotBase::SetPosition( float X, float Y, float Z )
{
	m_buffer.m_position = { X, Y, Z };
}

const DirectX::XMFLOAT4 & LightSpotBase::GetColor() const
{
	return m_buffer.m_colorAndIntensity;
}

float LightSpotBase::GetIntensity() const
{
	return m_buffer.m_colorAndIntensity.w;
}

void LightSpotBase::SetColor( float R, float G, float B )
{
	m_buffer.m_colorAndIntensity = { R,G,B,m_buffer.m_colorAndIntensity.w };
}

void LightSpotBase::SetIntensity( float Intensity )
{
	m_buffer.m_colorAndIntensity.w = Intensity;
}

void LightSpotBase::SetInnerConeAngle( float InnerAngle )
{
	m_buffer.m_innerCone = InnerAngle;
}

void LightSpotBase::SetOuterConeAngle( float OuterAndle )
{
	m_buffer.m_outerCone = OuterAndle;
}

float LightSpotBase::GetInnerConeAngle()const
{
	return m_buffer.m_innerCone;
}

float LightSpotBase::GetOuterConeAngle()const
{
	return m_buffer.m_outerCone;
}

const SpotLightBuffer * LightSpotBase::GetDataBuffer() const
{
	return &m_buffer;
}
