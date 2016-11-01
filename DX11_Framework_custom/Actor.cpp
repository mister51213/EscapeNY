#include "Actor.h"
#include "MathUtils.h"

using namespace DirectX;

Actor::Actor( const ModelSpecs_W & worldSpecs, eTexture tex, const ModelSpecs_L & localSpecs, eModType modType )
	:
	m_worldSpecs( worldSpecs ),
	m_localSpecs( localSpecs ),
	m_texIndex( tex ),
	m_modType( modType )
{}

void Actor::UpdateState(eState state, float deltaT)
{
    m_state = state;
        switch (m_state)
        {
        case Stationary:
            return;
        case Falling:
            {
                DoGravity(deltaT);
            }
        case Moving:
            {
                Move(deltaT);
            }
        case Move_PID:
            {
                MovePID(deltaT); // TODO: change this to non-default parameter
            }
        default:
            return;
        }
}

void Actor::Move( const float deltaT)
{
    // TODO:
    // 1. Properly implement time (get actual time elapsed from system)
    // 2. Need to initialize velocity elsewhere?

    //XMFLOAT3 deltaPos = m_physics.m_attributes.velocity * deltaT;
	XMFLOAT3 deltaPos = m_attributes.velocity * deltaT;
    m_worldSpecs.position += deltaPos;
}

// TODO: THESE are currently not used; must stop using the member m_physics variable in Actor.
void Actor::MovePID(float deltaT)
{
	m_worldSpecs.position = m_physics.MoveToTarget_ALT( m_worldSpecs, m_attributes, deltaT);
}

// ALT VERSION 2
void Actor::MoveToTarget_ALT2(const float deltaT )
{
	const float gainCoefficient = 1.f;
	m_attributes.posError = m_attributes.target - m_worldSpecs.position;
	XMFLOAT3 increment = m_attributes.posError * gainCoefficient * deltaT;

	m_worldSpecs.position += increment;
}

void Actor::DoGravity( const float deltaT)
{
	m_worldSpecs.position = m_physics.ApplyGravity(m_worldSpecs,m_attributes,deltaT);
}
