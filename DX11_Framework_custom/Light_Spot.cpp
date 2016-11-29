#include "Light_Spot.h"

using namespace DirectX;

const DirectX::XMFLOAT3 & Light_Spot::GetDirection() const
{
	return m_lightBuffer.lightDirection;
}

void Light_Spot::SetDirection( float X, float Y, float Z )
{
	m_lightBuffer.lightDirection = { X, Y, Z };
}

const DirectX::XMFLOAT3 & Light_Spot::GetPosition() const
{
	return m_lightBuffer.lightPosition;
}

void Light_Spot::SetPosition( const XMFLOAT3 & position )
{
	m_lightBuffer.lightPosition = position;
}

const DirectX::XMFLOAT4 & Light_Spot::GetColor() const
{
	return m_lightBuffer.lightColor;
}

void Light_Spot::SetConeAngle( float OuterAndle )
{
	m_lightBuffer.coneAngle = OuterAndle;
}

float Light_Spot::GetConeAngle() const
{
	return m_lightBuffer.coneAngle;
}