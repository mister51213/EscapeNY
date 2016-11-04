#include "Actor_Dynamic.h"



Actor_Dynamic::Actor_Dynamic(ModelSpecs_W worldSpecs, eTexture tex, ModelSpecs_L localSpecs, eModType modType): 
	Actor( worldSpecs, tex, localSpecs, modType )
{}

void Actor_Dynamic::SetState(eActorState state)
{
	    m_state = state;
}

Actor_Dynamic::eActorState Actor_Dynamic::GetState() const
{
	return m_state;
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
	// Temporary lambda functions comparing difference in 
	// implementation of acceleration.

	auto JoshChaseTarget = [ & ]()
	{
		if ( m_target != XMFLOAT3{ 0.f, 0.f, 0.f } )
		{
			const float constSpeed = 50.f;

			const XMFLOAT3 range = m_target - m_initalPosition;
			const float totalDistance = Magnitude( range );

			const XMFLOAT3 toTargetVector = m_target - m_worldSpecs.position;
			const float distanceToTarget = Magnitude( toTargetVector );
			const float inverseDistance = 1.f / distanceToTarget;

			if ( distanceToTarget <= 0.01f )
			{
				m_initalPosition = m_target;
				return;
			}

			const float accelCoeff = distanceToTarget / totalDistance;
			const float decelCoeff = ( 1.f - accelCoeff );

			const XMFLOAT3 direction = toTargetVector * inverseDistance;
			const XMFLOAT3 frameVelocity = direction * ( accelCoeff * constSpeed );
			const XMFLOAT3 frameDecelVel = -direction * ( decelCoeff * constSpeed );

			const float timeSquared = Square( deltaT );
			const XMFLOAT3 acceleration = frameVelocity * timeSquared;
			const XMFLOAT3 deceleration = frameDecelVel * timeSquared;
			const XMFLOAT3 finalAcceleration = acceleration + deceleration;

			m_attributes.velocity += finalAcceleration;
			m_worldSpecs.position += m_attributes.velocity;
			int a = 0;
		}
	};

	auto GregChaseTarget = [ & ]()
	{
		//XMFLOAT3 increment = posError * gainCoefficient * deltaT;
		//m_worldSpecs.position += increment;

		XMFLOAT3 posError = m_target - m_worldSpecs.position;
		const float gainCoefficient = 10.f;
		const float integrator = 0.5f;

		XMFLOAT3 displacement = { 0.f, 0.f, 0.f };
		XMFLOAT3 acceleration = { 0.f, 0.f, 0.f };

		XMFLOAT3 recipPosError = posError * 0.9;
		if ( posError.x != 0.0f && posError.y != 0.0f && posError.z != 0.0f )
		{
			recipPosError =
			{
				1.0f / posError.x,
				1.0f / posError.y,
				1.0f / posError.z
			};
		}

		// increment velocity by fraction of distance to target
		acceleration = ( posError - recipPosError ) * gainCoefficient;
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
	};
		
	JoshChaseTarget();
	//GregChaseTarget();
}

XMFLOAT3 Actor_Dynamic::MoveTowardTarget( const float deltaT )
{
	// will calculate and add this displacement to actual position
	
	// Store current position
	XMFLOAT3 currPos = m_worldSpecs.position;

	// How far do we still have to travel?
	XMFLOAT3 posError = m_target - currPos;

	// Magnitude will always be positive, so no need for call to abs()
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
	const XMFLOAT3 deltaPos = ( ( m_attributes.velocity*deltaT ) + ( requiredAccel*( deltaT*deltaT ) ) * 0.5f ) * dampener;
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