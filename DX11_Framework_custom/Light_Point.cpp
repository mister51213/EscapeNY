#include "Light_Point.h"
#include "Includes.h"

using namespace DirectX;

const XMFLOAT3 & Light_Point::GetPosition() const
{
	return m_lightBuffer.lightPosition;
}

void Light_Point::SetPosition( const XMFLOAT3 & position )
{
	m_lightBuffer.lightPosition = position;
}
