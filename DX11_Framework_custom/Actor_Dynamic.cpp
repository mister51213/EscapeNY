#include "Actor_Dynamic.h"



Actor_Dynamic::Actor_Dynamic()
{}

Actor_Dynamic::Actor_Dynamic(ModelSpecs_W worldSpecs, eTexture tex, ModelSpecs_L localSpecs, eModType modType): 
	Actor( worldSpecs, tex, localSpecs, modType )
{}

Actor_Dynamic::~Actor_Dynamic()
{}

void Actor_Dynamic::SetState(eActorState state)
{
	    m_state = state;
}

void Actor_Dynamic::Update(float deltaT)
{
        switch (m_state)
        {
        case Stationary:
            return;
        case Falling:
            {

            }
        case Move_PID:
            {
                MoveToTarget(deltaT); // TODO: change this to non-default parameter
				break;
            }
		case Moving:
            {
                Move(deltaT);
				break;
            }
        default:
            return;
        }
}

void Actor_Dynamic::Move( const float deltaT)
{
	XMFLOAT3 deltaPos = m_attributes.velocity * deltaT;
    m_worldSpecs.position += deltaPos;
}

void Actor_Dynamic::MoveToTarget(const float deltaT )
{
	const float gainCoefficient = 1.5f;
	XMFLOAT3 m_posError = m_target - m_worldSpecs.position;
	XMFLOAT3 increment = m_posError * gainCoefficient * deltaT;

	m_worldSpecs.position += increment;
}

XMFLOAT3 Actor_Dynamic::MoveTowardTarget( const float deltaT )
{
	// will calculate and add this displacement to actual position
	XMFLOAT3 deltaPos = { 0.0f, 0.0f, 0.0f };

	// Store current position
	XMFLOAT3 currPos = m_worldSpecs.position;

	// How far do we still have to travel?
	XMFLOAT3 posError = m_target - currPos;
	float distToTarget = abs( Magnitude( posError ) );

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
	XMFLOAT3 requiredVeloc = posError * recipTime; // need to multiply this here?
	XMFLOAT3 requiredAccel = ( requiredVeloc - m_attributes.velocity ) * recipTime;

	float dampener = 0.05f;
	// Apply required accel and velocity and calculate displacement
	deltaPos = ( ( m_attributes.velocity*deltaT ) + ( requiredAccel*( deltaT*deltaT ) ) * 0.5f ) * dampener;
	m_attributes.velocity += ( requiredAccel*deltaT );
	//}
	//else // kick in the integrator for fine tuning
	//{
	//}
	// Add displacement to ACTUAL position
	if ( distToTarget > 0.001 )
	{
		currPos += deltaPos;
	}
	else
	{
		currPos = m_target;
	}

	return currPos;
}