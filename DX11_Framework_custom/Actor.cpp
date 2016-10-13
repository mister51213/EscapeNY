#include "Actor.h"

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
            DoGravity(deltaT);
        case Moving:
            Move(deltaT);
        default:
            return;
        }
}

void Actor::Move( const float deltaT)
{
    // TODO:
    // 1. Properly implement time (get actual time elapsed from system)
    // 2. Need to initialize velocity elsewhere?

    XMFLOAT3 deltaPos = m_velocity * deltaT;
    m_worldSpecs.position += deltaPos;
}

// TODO:
/* Review this function: 
1. Is displacement formula working properly?
(the second part of the sum; it's just multiplying by gravity but not latest velocity value)
2. Is it working properly in conjunction w the Move function, which also applies velocity
3. Will the y value always point down in world space? (review inertial space vs world space)
4. Is this the most efficient implementation?
5. Properly implement time (get actual time elapsed from system)*/
// TODO

void Actor::DoGravity( const float deltaT)
{
// Given an initial position, velocity, and constant acceleration over a given 
// time, use the displacement formula to calculate the new instantaneous position. 
// *****Only acts on the Y component of the world position value of the object****
        // calculate incremental displacement
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
    // update velocity
    m_velocity.y += m_gravity * deltaT;
}
