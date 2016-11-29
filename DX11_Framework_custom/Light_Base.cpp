#include "Light_Base.h"

Light_Base::Light_Base( eLightType Type )
{
	m_lightBuffer.type = Type;
}

const DirectX::XMFLOAT4 & Light_Base::GetColor() const
{
	return m_lightBuffer.lightColor;
}

void Light_Base::SetColor( float R, float G, float B )
{
	m_lightBuffer.lightColor = { R, G, B, m_lightBuffer.lightColor.w };
}

float Light_Base::GetIntensity() const
{
	return m_lightBuffer.lightColor.w;
}

void Light_Base::SetIntensity( float Value )
{
	m_lightBuffer.lightColor.w = Value;
}

const LightBufferType & Light_Base::GetLightBufferType() const
{
	return m_lightBuffer;
}
