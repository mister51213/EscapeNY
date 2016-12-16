#include "Actor.h"
#include "MathUtils.h"

using namespace DirectX;

Actor::Actor( const ModelSpecs_W & worldSpecs, eTexture tex, const ModelSpecs_L & localSpecs, eModType modType, std::vector<DirectX::XMFLOAT3>* pNormMesh )
	:
	m_worldSpecs( worldSpecs ),
	m_localSpecs( localSpecs ),
	m_texIndex( tex ),
	m_modType( modType ),
	m_AABBox(worldSpecs, pNormMesh)
{}

Actor::~Actor()
{}

eModType Actor::GetModelType() const
{
	return m_modType;
}

eTexture Actor::GetTexIndex() const
{
	return m_texIndex;
}

const ModelSpecs_W & Actor::GetWorldSpecs() const
{
	return m_worldSpecs;
}

const ModelSpecs_L & Actor::GetLocalSpecs() const
{
	return m_localSpecs;
}

PhysAttributes & Actor::GetAttributes()
{
	return m_attributes;
}

void Actor::SetWorldSpecs( const ModelSpecs_W & newSpecs )
{
	m_worldSpecs = newSpecs;
}

void Actor::SetPosition( const XMFLOAT3 & newPos )
{
	m_worldSpecs.position = newPos;
}

const XMFLOAT3 & Actor::GetPosition() const
{
	return m_worldSpecs.position;
}

const XMFLOAT3 & Actor::GetRotation() const
{
	return m_worldSpecs.orientation;
}

void Actor::GetInput( Input & pInput, int randI, float randF )
{}
