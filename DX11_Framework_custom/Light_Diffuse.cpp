#include "Light_Diffuse.h"

const DirectX::XMFLOAT3 & Light_Diffuse::GetDirection() const
{
	return m_lightBuffer.lightDirection;
}

void Light_Diffuse::SetDirection( float X, float Y, float Z )
{
	m_lightBuffer.lightDirection = { X, Y, Z };
}
