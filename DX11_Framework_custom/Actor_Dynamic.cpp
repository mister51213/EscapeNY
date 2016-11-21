#include "Actor_Dynamic.h"



Actor_Dynamic::Actor_Dynamic(ModelSpecs_W worldSpecs, eTexture tex, ModelSpecs_L localSpecs, eModType modType): 
	Actor( worldSpecs, tex, localSpecs, modType )
{
	ResetPIDParams();
}

void Actor_Dynamic::SetState(eActorState state)
{
	    m_state = state;
}

Actor_Dynamic::eActorState Actor_Dynamic::GetState() const
{
	return m_state;
}

void Actor_Dynamic::ResetPIDParams( XMFLOAT3 target )
{		
	m_target = target;
	m_initalPosition = GetPosition();
	m_initialHeading = m_target - m_initalPosition;
	m_halfway = abs(Magnitude(m_initialHeading)) * 0.5;
}

void Actor_Dynamic::Update(float deltaT)
{
        switch (m_state)
        {
        case STILL:
		{
			m_attributes.velocity = { 0.f, 0.f, 0.f };
			return;
		}
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
	auto crudeOption = [ & ]()
	{
	//XMFLOAT3 posError = m_target - m_worldSpecs.position;
	//XMFLOAT3 increment = 1.f * deltaT;
	//m_worldSpecs.position += increment;
	};

	auto bestOption = [ & ]()
	{
		XMFLOAT3 currDistV = m_target - m_worldSpecs.position;
		float error = Magnitude( currDistV );
		XMFLOAT3 dirToTarget = Normalize(currDistV);
		
		//////////// PROPORTIONAL ACCELERATION METHOD //////////////
		float pCoeff = 0.2f; // proportional gain coefficient
		float dCoeff = -.07f; // derivative gain coefficient
		m_integrator += error; // TODO: add conditional statement to apply this
		/*
		NOTE: Integrator is only needed if real world friction applies.
		In other words, if the thrust input doesn't produce the velocity
		that you expect, because some of it is lost to friction, then you
		have to creep the rest of the way to the goal using the integrator.

		So to add another level of realism, this function shouldnt be able to
		automatically increment acceleration and velocity, but rather input a force, 
		and the result of that force will be affected by other forces in the world, 
		such as friction and drag. Then it will check its own resultant velocity, 
		and continue to calculate	how much input it needs to give based on that. 
		
		This is how PID works in the real world.
		*/
		m_attributes.acceleration = dirToTarget * error * pCoeff;
		m_attributes.acceleration += m_attributes.velocity * dCoeff;
		// TODO: Add friction to damp the speed as it approaches target;
		// TODO: that way, we won't only rely on P and D coefficients

		//////////// PROPORTIONAL ACCELERATION METHOD //////////////

		////////// UNIVERSAL FINAL CALCULATION ///////
		m_attributes.velocity += m_attributes.acceleration;
		XMFLOAT3 displacement = m_attributes.velocity * deltaT;
		m_worldSpecs.position += displacement;
	};

	auto betterOption = [ & ]()
	{
					/*
		STEP ONE
			*must CLAMP the proportional gain below:
		acceleration = error * gainCoefficient; (a positive const value)
		acceleration += velocity * derivativeGainCoefficient; (a negative const value)

		STEP TWO
			*add friction to damp the oscillation
		*/
		XMFLOAT3 currDistV = m_target - m_worldSpecs.position;
		float error = Magnitude( currDistV );
		XMFLOAT3 dirToTarget = Normalize(currDistV);
		
		/////////// FIXED ACCELERATION METHOD //////////////////
		float accelCoeff = 3.5f;

		if ( error < m_halfway )
		{
			accelCoeff *= -1.0;
		}
		if ( error < 5.0f ) //CLOSE RANGE
		{
			accelCoeff = 0.f;
			m_attributes.velocity = { 0.f, 0.f, 0.f };
		}
		/////////// FIXED ACCELERATION METHOD //////////////////

		// THE VELOCITY IS THE OUTCOME OF INTEGRATING THE ACCELERATION OVER TIME
		m_attributes.acceleration = dirToTarget * accelCoeff;
		m_attributes.velocity += m_attributes.acceleration;
		XMFLOAT3 displacement = m_attributes.velocity * deltaT;
		m_worldSpecs.position += displacement;
	};

	auto optionThree = [ & ]()
	{
			// You can consider this the amount of force that is being applied to the object.  
		const float constSpeed = 5.f;

		// My understanding of the problem can be solved using linear
		// interpolation of the amount of acceleration in opposing
		// directions, but you need to know your starting position and
		// ending position for it to work.

		// Linear interpolation is where you calculate the position
		// along a line based on some percentage.  
		/*
			If you have a starting point of (0, 0, 0) and want to end at
			(100, 0, 0) you can see that when you are 25% done with your
			journey, you will be at (25, 0, 0) and you still have 75% to go.
			So the vector that is left is (75, 0, 0).

			The formula for linear interpolation (LERP) is:
			completed = start + ((end - start) * percentage)
			remaining = start + ((end - start) * (1.0-percentage))
			total = completed + remaining
			Percentage is usually between 0.0 and 1.0
		*/

		// range is the (end - start) in vector form and total distance
		// is the scalar form
		const XMFLOAT3 range = m_target - m_initalPosition;
		const float totalDistance = Magnitude( range );

		// toTargetVector and distanceToTarget are just intermediary
		// steps to find the percentage part
		const XMFLOAT3 toTargetVector = m_target - m_worldSpecs.position;
		const float distanceToTarget = Magnitude( toTargetVector );
		const float inverseDistance = 1.f / distanceToTarget;

		// accelCoeff is the percentage portion of the LERP formula
		const float accelCoeff = distanceToTarget / totalDistance;
		// decelCoeff is the remaining percentage portion of the LERP formula
		const float decelCoeff = ( 1.f - accelCoeff );

		if ( decelCoeff >= 1.f )
		{
			int a = 0;
		}

		// Calculates the direction of the force to be applied.
		const XMFLOAT3 direction = toTargetVector * inverseDistance;

		// accelCoeff * constSpeed, is the magnitude of displacement to be
		// applied in the direction the object is going.
		const XMFLOAT3 frameVelocity = direction * ( accelCoeff * constSpeed );

		// decelCoeff * constSpeed, is the magnitude of displacement to be
		// applied in the opposing direction the object is going.
		const XMFLOAT3 frameDecelVel = -direction * ( decelCoeff * constSpeed );

		const float timeSquared = Square( deltaT );

		// acceleration is m/s2, so that should explain these steps
		const XMFLOAT3 acceleration = frameVelocity * timeSquared;
		const XMFLOAT3 deceleration = frameDecelVel * timeSquared;

		// This finishes off the LERP formula, where accleration + deceleration
		// equals 100% of the total range.
		const XMFLOAT3 finalAcceleration = acceleration + deceleration;

		// We increase velocity by the amount of desired acceleration for
		// this frame.
		m_attributes.velocity += finalAcceleration;

		// We add velocity to position as the total amount of displacement
		// for this frame.
		m_worldSpecs.position += m_attributes.velocity;
	};
	auto JoshChaseTarget = [ & ]()
	{
		if ( m_target != XMFLOAT3{ 0.f, 0.f, 0.f } )
		{
			const XMFLOAT3 range = m_target - m_initalPosition;
			const float totalDistance = Magnitude( range );

			const XMFLOAT3 toTargetVector = m_target - m_worldSpecs.position;
			const float distanceToTarget = Magnitude( toTargetVector );
			const float inverseDistance = 1.f / distanceToTarget;

			//if ( distanceToTarget <= 0.01f )
			//{
			//	m_initalPosition = m_target;
			//	return;
			//}

			const float accelCoeff = distanceToTarget / totalDistance;
			const float decelCoeff = -( 1.f - accelCoeff );

			XMFLOAT3 velToAdd = m_attributes.velocity * accelCoeff * .3f;
			XMFLOAT3 velToSub = m_attributes.velocity * decelCoeff * .3f;
			XMFLOAT3 nudge = range * 0.01f;

			float halfRange = totalDistance * 0.5f;
			float closeRange = totalDistance * 0.3f;
			XMFLOAT3 displacement;

			if ( abs(distanceToTarget) > abs(halfRange) ) // accelerate
			{
			}
			//else if ( abs(distanceToTarget) < abs(closeRange) ) // tractor beam
			//{
			//	m_attributes.velocity = { 0.f, 0.f, .0f };
			//	displacement = range * deltaT * 1.5f;
			//}
			else
			{
			}

			// ACCELERATE
			m_attributes.velocity += velToAdd + nudge;
			m_attributes.velocity += velToSub; // hit the breaks

			displacement = m_attributes.velocity * deltaT;
			m_worldSpecs.position += displacement;

			//const XMFLOAT3 direction = toTargetVector * inverseDistance;

			//// velocityToAdd is the instantaneous velocity that we must add to reach the target
			//const XMFLOAT3 velocityToAdd = direction * ( accelCoeff * constSpeed );
			//const XMFLOAT3 velocityToSubtract = -direction * ( decelCoeff * constSpeed );

			//const float timeSquared = Square( deltaT );
			//const XMFLOAT3 acceleration = velocityToAdd * timeSquared;
			//const XMFLOAT3 deceleration = velocityToSubtract * timeSquared;

			//const XMFLOAT3 finalAcceleration = acceleration + deceleration;

			//m_attributes.velocity += finalAcceleration;
			//m_worldSpecs.position += m_attributes.velocity;

			//int a = 0;
		}
	};
	auto GregChaseTarget = [ & ]()
	{
		XMFLOAT3 posError = m_target - m_worldSpecs.position;
		const float integrator = 0.5f;

		XMFLOAT3 displacement = { 0.f, 0.f, 0.f };
		XMFLOAT3 acceleration = { 0.f, 0.f, 0.f };

		XMFLOAT3 recipPosError =
		{ 
			posError.x == 0.0f ? 0.0f : 1.0f / posError.x,
			posError.x == 0.0f ? 0.0f : 1.0f / posError.y,
			posError.x == 0.0f ? 0.0f : 1.0f / posError.z 
		};

		//if ( posError.x != 0.0f && posError.y != 0.0f && posError.z != 0.0f )
		//{
		//	recipPosError =
		//	{
		//		1.0f / posError.x,
		//		1.0f / posError.y,
		//		1.0f / posError.z
		//	};
		//}

		const float gainCoefficient = 20.f;
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
		
	//JoshChaseTarget();
	//GregChaseTarget();
	//optionThree();
	//betterOption();
	//crudeOption();
	bestOption();
}

void Actor_Dynamic::Stop()
{
	m_state = STILL;
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