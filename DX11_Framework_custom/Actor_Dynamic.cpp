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
        case STILL:
            return;
        case FALLING:
            {

            }
        case HOMING:
            {
                ChaseTarget(deltaT); // TODO: change this to non-default parameter
				break;
            }
		case CONST_MOVE:
            {
                ConstantMove(deltaT);
				break;
            }
        default:
            return;
        }
}

void Actor_Dynamic::ConstantMove( const float deltaT)
{
	XMFLOAT3 deltaPos = m_attributes.velocity * deltaT;
    m_worldSpecs.position += deltaPos;
}

void Actor_Dynamic::ChaseTarget(const float deltaT )
{
	//XMFLOAT3 increment = posError * gainCoefficient * deltaT;
	//m_worldSpecs.position += increment;

	XMFLOAT3 posError = m_target - m_worldSpecs.position;	
	const float gainCoefficient = 10.f;
	const float integrator = 0.5f;

	XMFLOAT3 displacement = { 0.f, 0.f, 0.f };
	XMFLOAT3 acceleration = { 0.f, 0.f, 0.f };

	XMFLOAT3 recipPosError = posError * 0.9;
	if (posError.x !=0.0f && posError.y !=0.0f && posError.z !=0.0f )
	{
		recipPosError = 
		{
			1.0f / posError.x,
			1.0f / posError.y,
			1.0f / posError.z
		};
	}

			// increment velocity by fraction of distance to target
		acceleration = (posError - recipPosError) * gainCoefficient;
		XMFLOAT3 velocityChange = acceleration * deltaT;
		m_attributes.velocity += velocityChange;

		//if ( Magnitude( posError ) < 50.f ) // hit the breaks
		//	m_attributes.velocity *= integrator;
		
	//if(Magnitude(posError) > 100.f)
	//{
	//	// increment velocity by fraction of distance to target
	//	acceleration = (posError - recipPosError);
	//	XMFLOAT3 velocityChange = acceleration * deltaT * gainCoefficient;
	//	m_attributes.velocity += velocityChange;
	//}
	//else
	//{
	//	// creep toward target until you reach it
	//	//m_attributes.velocity = posError * integrator *deltaT;
	//	acceleration = (posError - recipPosError);
	//	XMFLOAT3 velocityChange = acceleration * deltaT * integrator;
	//	m_attributes.velocity += velocityChange;
	//}

	displacement = m_attributes.velocity * deltaT;
	m_worldSpecs.position += displacement;	
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