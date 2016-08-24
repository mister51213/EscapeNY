#include "LightPoint.h"



LightPoint::LightPoint()
{
}


LightPoint::~LightPoint()
{
}

const DirectX::XMFLOAT4 & LightPoint::GetAmbient() const
{
	return m_buffer.m_ambient;
}

void LightPoint::SetAmbient( float R, float G, float B )
{
	m_buffer.m_ambient = { R, G, B, 1.f };
}

const DirectX::XMFLOAT4 & LightPoint::GetColor() const
{
	return m_buffer.m_diffuse;
}

void LightPoint::SetColor( float R, float G, float B )
{
	m_buffer.m_diffuse = { R, G, B, 1.f };
}

float LightPoint::GetIntensity() const
{
	return m_buffer.m_intensity;
}

void LightPoint::SetIntensity( float Value )
{
	m_buffer.m_intensity = Value;
}

void LightPoint::SetPosition( float X, float Y, float Z )
{
	m_buffer.m_position = { X, Y, Z };
}

DirectX::XMFLOAT3 LightPoint::GetPosition() const
{
	return m_buffer.m_position;
}
