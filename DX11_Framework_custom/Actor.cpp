#include "Actor.h"

Actor::Actor( const ModelSpecs_W & worldSpecs, eTexture tex, const ModelSpecs_L & localSpecs, eModType modType )
	:
	m_worldSpecs( worldSpecs ),
	m_localSpecs( localSpecs ),
	m_texIndex( tex ),
	m_modType( modType )
{}

void Actor::Move(float& const deltaT)
{
    XMFLOAT3 deltaPos = m_velocity * deltaT;
    m_worldSpecs.position += deltaPos;
}

void Actor::DoGravity(float& const deltaT)
{
// Given an initial position, velocity, and constant acceleration over a given 
// time, use the displacement formula to calculate the new instantaneous position. 
// *****Only acts on the Y component of the world position value of the object****

    if (m_velocity.y < terminalVelocity)
    {
        float deltaPosY = (m_velocity.y*deltaT) + (m_gravity*(deltaT*deltaT)) / 2.0f;
        m_worldSpecs.position.y += deltaPosY;
    }
    else // Stop accelerating at terminal velocity
    {
        float deltaPosY = m_worldSpecs.position.y * deltaT;
        m_worldSpecs.position.y += deltaPosY;
    }
}
