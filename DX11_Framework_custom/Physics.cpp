#include "Physics.h"
#include "MathUtils.h"

using namespace DirectX;

void Physics::MakeBoundingBox(BoundingBox bounds, eModType type)
{
	if( type == SPHERE)
	{
	}
}

bool Physics::CheckCollision(BoundingBox bounds)
{
	return false;
}

bool Physics::CorrectCollision()
{
	return false;
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

// ALT VERSION
XMFLOAT3 Physics::MoveToTarget( const ModelSpecs_W& worldSpecs, const float deltaT )
{
	const float gainCoefficient = 1.f;
	m_attributes.posError = m_attributes.target - worldSpecs.position;
	
	XMFLOAT3 increment = m_attributes.posError * gainCoefficient * deltaT;
	return worldSpecs.position + increment;
}

XMFLOAT3 Physics::MoveTowardTarget( const ModelSpecs_W& worldSpecs, const float deltaT )
{
	// will calculate and add this displacement to actual position
	XMFLOAT3 deltaPos = { 0.0f, 0.0f, 0.0f };

	// Store current position
	XMFLOAT3 currPos = worldSpecs.position;

	// How far do we still have to travel?
	m_attributes.posError = m_attributes.target - currPos;
	float distToTarget = abs( Magnitude( m_attributes.posError ) );

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
	XMFLOAT3 requiredVeloc = m_attributes.posError * recipTime; // need to multiply this here?
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
	if ( distToTarget > 0.01 )
	{
		currPos += deltaPos;
	}
	else
	{
		currPos = m_attributes.target;
	}

	return currPos;
}

XMFLOAT3 Physics::ApplyGravity( const ModelSpecs_W& worldSpecs, const float deltaT )
{
	// Given an initial position, velocity, and constant acceleration over a given 
	// time, use the displacement formula to calculate the new instantaneous position. 
	// *****Only acts on the Y component of the world position value of the object****

	// calculate incremental displacement
	XMFLOAT3 currPosition = worldSpecs.position;
	if ( m_attributes.velocity.y < m_attributes.terminalV )
	{
		float deltaPosY = ( m_attributes.velocity.y*deltaT ) + ( Gravity*( deltaT*deltaT ) ) * 0.5f;
		currPosition.y += deltaPosY;
	}
	else // Stop accelerating at terminal velocity
	{
		float deltaPosY = currPosition.y * deltaT;
		currPosition.y += deltaPosY;
	}
	// update velocity
	m_attributes.velocity.y += Gravity * deltaT;

	return currPosition;
}

XMFLOAT3 Physics::ApplyForce( eForceType forceType )
{
	return XMFLOAT3();
}

