#pragma once
#include "ILightSpot.h"

class Light_Spot :
    public ILightSpot
{
public:

    Light_Spot()
    {
    }

    ~Light_Spot()
    {
    }

const DirectX::XMFLOAT3 & Light_Spot::GetDirection() const override
{
	return m_lightBuffer.lightDirection;
}

void Light_Spot::SetDirection( float X, float Y, float Z ) override
{
	m_lightBuffer.lightDirection = { X, Y, Z };
}

const DirectX::XMFLOAT3 & Light_Spot::GetPosition() const override
{
	return m_lightBuffer.lightPosition;
}

void Light_Spot::SetPosition( const XMFLOAT3& position ) override
{
	m_lightBuffer.lightPosition = position;
}

const DirectX::XMFLOAT4 & Light_Spot::GetColor() const override
{
	return m_lightBuffer.lightColor;
}

float Light_Spot::GetIntensity() const override
{
	return m_lightBuffer.lightColor.w;
}

void Light_Spot::SetColor( float R, float G, float B ) override
{
	m_lightBuffer.lightColor = { R,G,B,m_lightBuffer.lightColor.w };
}

void Light_Spot::SetIntensity( float Intensity ) override
{
	m_lightBuffer.lightColor.w = Intensity;
}

////////////////////////
// SPOTLIGHT SPECIFIC //
////////////////////////

void Light_Spot::SetConeAngle( float OuterAndle ) override
{
	m_lightBuffer.coneAngle = OuterAndle;
}

float Light_Spot::GetConeAngle()const override
{
	return m_lightBuffer.coneAngle;
}

const LightBufferType& Light_Spot::GetLightBufferType() const override
{
	return m_lightBuffer;
}

private:
    LightBufferType m_lightBuffer;
};

