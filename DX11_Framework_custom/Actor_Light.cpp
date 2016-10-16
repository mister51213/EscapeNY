#include "Actor_Light.h"

Actor_Light::Actor_Light()
{

}

void Actor_Light::Initialize(const XMFLOAT3 &Position, const XMFLOAT3 &LookAt)
{
		m_spotLight.SetIntensity( .7f );
		m_spotLight.SetPosition( Position );
		auto dir = Normalize( LookAt - Position );
        m_spotLight.SetDirection( dir.x, dir.y, dir.z);
}

Actor_Light::~Actor_Light()
{
}

