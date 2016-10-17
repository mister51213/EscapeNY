#include "Actor_Light.h"

void Actor_Light::InitSpotLight(const Camera & Cam)
{	
	SetPosition(Cam.GetPosition());
	SetOrientation(Cam.GetRotation());
	m_spotLight.SetColor(1.0f, 1.0f, 1.0f);
}

void Actor_Light::SetPosition(const XMFLOAT3 &Pos)
{
	m_worldSpecs.position = Pos + XMFLOAT3(1.f, -1.f, 0.f);
}

void Actor_Light::SetOrientation(const XMFLOAT3 &Dir)
{
	m_worldSpecs.orientation = Dir;
}

ILight * Actor_Light::GetLight()
{
	auto xmOriV = XMLoadFloat3(&m_worldSpecs.orientation);
	xmOriV = ConvertToRadians(xmOriV);

	auto xmRotM = XMMatrixRotationRollPitchYawFromVector(xmOriV);

	auto xmForwardV = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	xmOriV = XMVector3Transform(xmForwardV, xmRotM);

	XMFLOAT3 oriV{};
	XMStoreFloat3(&oriV, xmOriV);
	m_spotLight.SetDirection(oriV);

	return &m_spotLight;
}