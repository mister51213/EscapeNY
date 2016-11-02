#include "Physics.h"
#include "MathUtils.h"

#ifdef min || max
#undef min
#undef max
#endif

using namespace DirectX;

ModelSpecs_W Physics::UpdateAll( PhysAttributes & attributes )
{
	return ModelSpecs_W();
}

// ALT VERSION
XMFLOAT3 Physics::MoveToTarget_ALT( const ModelSpecs_W & worldSpecs, PhysAttributes & attributes, const float deltaT ) const
{
	const float gainCoefficient = 1.f;
	attributes.posError = attributes.target - worldSpecs.position;
	XMFLOAT3 increment = attributes.posError * gainCoefficient * deltaT;

	return worldSpecs.position + increment;
}

void Physics::MakeBoundingBox(BoundingBox bounds, eModType type)
{
	if( type == SPHERE)
	{
	}
}

AlignedAxisBoundingBox Physics::MakeBoundingBox( const std::vector<DirectX::XMFLOAT3> &VertexList)
{
	// Initialize the minimum and maximum values for all the vertices
	// to numbers that will definitely be overridden
	XMFLOAT3 minVertex{ FLT_MAX, FLT_MAX, FLT_MAX };
	XMFLOAT3 maxVertex{ FLT_MIN, FLT_MIN, FLT_MIN };

	// Loop through the list of vertices and collect the lowest and highest
	// values for each component
	for ( auto& vertex : VertexList )
	{
		minVertex.x = std::min( minVertex.x, vertex.x );
		minVertex.y = std::min( minVertex.y, vertex.y );
		minVertex.z = std::min( minVertex.z, vertex.z );

		maxVertex.x = std::max( maxVertex.x, vertex.x );
		maxVertex.y = std::max( maxVertex.y, vertex.y );
		maxVertex.z = std::max( maxVertex.z, vertex.z );
	}

	const auto extent = ( maxVertex - minVertex ) * .5f;
	const auto center = minVertex + extent;
	AlignedAxisBoundingBox bounds( center, extent );

	// Return the BoundingBox object.
	return bounds;
}

bool Physics::CheckCollision(BoundingBox bounds)
{
	return false;
}

bool Physics::CheckCollision( 
	const XMFLOAT3 &PositionA, const AlignedAxisBoundingBox &BoundsA,
	const XMFLOAT3 &PositionB, const AlignedAxisBoundingBox &BoundsB )
{
	// More information may be needed to determine how to handle the collision
	// after verifying that there is a collision, but for now this just 
	// determines if there was a collision.
	
	// If you don't like this implementation, then make sure that the center 
	// member of the two AlignedAxisBoundingBox objects have been translated
	// before entering this function.
	const auto aabbA = AlignedAxisBoundingBox( BoundsA.center + PositionA, BoundsA.extent );
	const auto aabbB = AlignedAxisBoundingBox( BoundsB.center + PositionB, BoundsB.extent );

	bool overlaps = aabbA.Overlaps( aabbB );
	return overlaps;
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


//////////////////////////////
//	NEW PHYS FUNCTIONS //
//////////////////////////////

XMFLOAT3 Physics::ApplyGravity_ALT( const float Mass, const float DeltaTime )
{
	// Constant gravity acceleration for free falling: -9.8f in the Y direction
	// This should be a constant in Utilities.h or MathUtils.h
	const float gravityConstant = -9.8f;

	const float timeSquared = DeltaTime * DeltaTime;
	const XMFLOAT3 accelerationDirection( 0.f, -gravityConstant, 0.f );
	const XMFLOAT3 acceleration( accelerationDirection * timeSquared );
	
	return acceleration * Mass;

	// Same rule would apply for attracting forces like two masses in space 
	// or particles being attraced to each other, just in the direction
	// of each other.
}
XMFLOAT3 Physics::ApplyWind( const float Mass )
{
	// Wind direction and strength (it's vector) could change each frame or each
	// level.  Depending on the wind speed, it may have no affect on player velocity

	// Mass of air is actually 1.19g per liter of air.  Assuming a volume of 
	// 12 cubic feet for the player (~6 foot tall lbs), the amount of mass 
	// hitting the player is 0.00936837378151260504201680672269 Kg

	const float airMass = 0.00936837f;
	const XMFLOAT3 windDirection( -1.f, 0.f, 0.f );
	return windDirection * airMass;
}

XMFLOAT3 Physics::DoPhysics( const float Mass, const float DeltaTime, const XMFLOAT3 &Velocity )
{
	XMFLOAT3 acceleration = ApplyGravity_ALT( Mass, DeltaTime );
	acceleration += ApplyWind( Mass );
	// Apply any physical forces desired, then add acceleration to velocity 
	// and return result

	return Velocity + acceleration;
}



XMFLOAT3 Physics::MoveToTarget( const ModelSpecs_W& worldSpecs, PhysAttributes& attributes, const float deltaT )
{
	const float gainCoefficient = 1.f;
	//m_attributes.posError = m_attributes.target - worldSpecs.position;
	attributes.posError = attributes.target - worldSpecs.position;
	//XMFLOAT3 increment = m_attributes.posError * gainCoefficient * deltaT;
	XMFLOAT3 increment = attributes.posError * gainCoefficient * deltaT;

	return worldSpecs.position + increment;
}

XMFLOAT3 Physics::MoveTowardTarget( const ModelSpecs_W& worldSpecs, PhysAttributes& attributes, const float deltaT )
{
	// will calculate and add this displacement to actual position
	XMFLOAT3 deltaPos = { 0.0f, 0.0f, 0.0f };

	// Store current position
	XMFLOAT3 currPos = worldSpecs.position;

	// How far do we still have to travel?
	attributes.posError = attributes.target - currPos;
	float distToTarget = abs( Magnitude( attributes.posError ) );

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
	XMFLOAT3 requiredVeloc = attributes.posError * recipTime; // need to multiply this here?
	XMFLOAT3 requiredAccel = ( requiredVeloc - attributes.velocity ) * recipTime;

	float dampener = 0.05f;
	// Apply required accel and velocity and calculate displacement
	deltaPos = ( ( attributes.velocity*deltaT ) + ( requiredAccel*( deltaT*deltaT ) ) * 0.5f ) * dampener;
	attributes.velocity += ( requiredAccel*deltaT );
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
		currPos = attributes.target;
	}

	return currPos;
}

XMFLOAT3 Physics::ApplyGravity( const ModelSpecs_W& worldSpecs, PhysAttributes& attributes, const float deltaT )
{
	// Given an initial position, velocity, and constant acceleration over a given 
	// time, use the displacement formula to calculate the new instantaneous position. 
	// *****Only acts on the Y component of the world position value of the object****

	// calculate incremental displacement
	XMFLOAT3 currPosition = worldSpecs.position;
	if ( attributes.velocity.y < attributes.terminalV )
	{
		float deltaPosY = ( attributes.velocity.y*deltaT ) + ( Gravity*( deltaT*deltaT ) ) * 0.5f;
		currPosition.y += deltaPosY;
	}
	else // Stop accelerating at terminal velocity
	{
		float deltaPosY = currPosition.y * deltaT;
		currPosition.y += deltaPosY;
	}
	// update velocity
	attributes.velocity.y += Gravity * deltaT;

	return currPosition;
}

XMFLOAT3 Physics::ApplyForce( eForceType forceType )
{
	return XMFLOAT3();
}

