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
            {
                DoGravity(deltaT);
            }
        case Moving:
            {
                Move(deltaT);
            }
        case Move_PID:
            {
                MovePID(m_target, deltaT); // TODO: change this to non-default parameter
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

    XMFLOAT3 deltaPos = m_velocity * deltaT;
    m_worldSpecs.position += deltaPos;
}

void Actor::MovePID(XMFLOAT3 targetPos, float deltaT)
    {
        // will calculate and add this displacement to actual position
        XMFLOAT3 deltaPos = { 0.0f, 0.0f, 0.0f };

        // Store current position
        XMFLOAT3 currPos = m_worldSpecs.position;

        // How far do we still have to travel?
        m_posError = targetPos - currPos;
        float distToTarget = abs(Magnitude(m_posError));

        // displacement by next frame if we continue traveling at current speed
        //XMFLOAT3 potentialDisp = m_velocity*deltaT;
        //XMFLOAT3 potentialPos = currPos + potentialDisp;

        //float potentialDist = abs(Magnitude(potentialPos - currPos));
        // Would we overshoot it traveling at this speed?
        /*if (abs(potentialDist) > abs(distToTarget))
        m_worldSpecs.position = targetPos;*/ // then just move us to the target

        // while further away than 20, full throttle toward target
        //if (distToTarget > 50.0f)
        //{
            float recipTime = 1.0f / deltaT;
            XMFLOAT3 requiredVeloc = m_posError * recipTime; // need to multiply this here?
            XMFLOAT3 requiredAccel = (requiredVeloc - m_velocity) * recipTime;
            
            float dampener = 0.01f;
            // Apply required accel and velocity and calculate displacement
            deltaPos = ((m_velocity*deltaT) + (requiredAccel*(deltaT*deltaT)) * 0.5f) * dampener;
            m_velocity += (requiredAccel*deltaT);
        //}
        //else // kick in the integrator for fine tuning
        //{
        //}
        // Add displacement to ACTUAL position
            if (distToTarget > 0.01)
            {
                m_worldSpecs.position += deltaPos;
            }
        else
        {
        m_worldSpecs.position = targetPos;
        }
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
        float deltaPosY = (m_velocity.y*deltaT) + (m_gravity*(deltaT*deltaT)) * 0.5f;
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
