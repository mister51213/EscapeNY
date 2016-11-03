#include "Actor_Light.h"
#include "MathUtils.h"

using namespace DirectX;

void Actor_Light::Initialize( const XMFLOAT3 &Position, const XMFLOAT3 &LookAt )
{
	m_spotLight.SetIntensity( .5f );
	m_spotLight.SetPosition( Position );
	const auto dir = Normalize( LookAt - Position );
	m_spotLight.SetDirection( dir.x, dir.y, dir.z );
}

void Actor_Light::SetLookat(const XMFLOAT3 &LookAt)
{
	const auto dir = Normalize( LookAt - m_spotLight.GetPosition() );
	m_spotLight.SetDirection( dir.x, dir.y, dir.z );
}
