#include "Physics.h"
#include "MathUtils.h"
#include "Utilities.h"

#if defined(min) || defined(max)
#undef min
#undef max
#endif

using namespace DirectX;

//ModelSpecs_W Physics::UpdateAll( PhysAttributes & attributes )
//{
//	return ModelSpecs_W();
//}

void Physics::ApplyImpulse( PhysAttributes & attributes, const float Mass, const float DeltaTime )
{}

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

bool Physics::CheckCollision( 
	const XMFLOAT3 &PositionA, const AlignedAxisBoundingBox &BoundsA,
	const XMFLOAT3 &PositionB, const AlignedAxisBoundingBox &BoundsB )
{
	// More information may be needed to determine how to handle the collision after verifying that 
	// there is a collision, but for now this just determines if there was a collision.
	
	// If you don't like this implementation, then make sure that the center member of the two 
	// AlignedAxisBoundingBox objects have been translated before entering this function.
	const auto aabbA = AlignedAxisBoundingBox( BoundsA.center + PositionA, BoundsA.extent );
	const auto aabbB = AlignedAxisBoundingBox( BoundsB.center + PositionB, BoundsB.extent );

	bool overlaps = aabbA.Overlaps( aabbB );
	return overlaps;
}

bool Physics::CorrectCollision()
{
	return false;
}

DirectX::XMFLOAT3 Physics::ApplyGravity( const float Mass, const float DeltaTime )
{
	// Constant gravity acceleration for free falling: -9.8f in the Y direction
	// This should be a constant in Utilities.h or MathUtils.h
	const float gravityConstant = -9.8f;

	const float timeSquared = DeltaTime * DeltaTime;
	const DirectX::XMFLOAT3 accelerationDirection( 0.f, -gravityConstant, 0.f );
	const DirectX::XMFLOAT3 acceleration( accelerationDirection * timeSquared );
	
	return acceleration * Mass; // RESULTANT INCREASE IN ACCELERATION 
	// Same rule would apply for attracting forces like two masses in space or particles 
	// being attraced to each other, just in the direction of each other.
}

DirectX::XMFLOAT3 Physics::ApplyWind( const float Mass, const float DeltaTime )
{
	// Wind direction and strength (it's vector) could change each frame or each
	// level.  Depending on the wind speed, it may have no affect on player velocity

	// Mass of air is actually 1.19g per liter of air.  Assuming a volume of 
	// 12 cubic feet for the player (~6 foot tall lbs), the amount of mass 
	// hitting the player is 0.00936837378151260504201680672269 Kg

	const float airMass = 0.00936837f;
	const DirectX::XMFLOAT3 windDirection( -1.f, 0.f, 0.f );
	return windDirection * airMass *DeltaTime; // RESULTANT INCREASE IN ACCELERATION 
}

void Physics::DoPhysics( PhysAttributes& attributes, const float DeltaTime)
{
	DirectX::XMFLOAT3 acceleration = ApplyGravity( attributes.mass, DeltaTime );
	acceleration += ApplyWind( attributes.mass, DeltaTime );
	//accelLin += ApplyForce1( Mass )*DeltaTime;
	//accelLin += ApplyForce2( Mass )*DeltaTime;

	// Apply any physical forces desired, then increment acceleration
	attributes.accelLin += acceleration;
	// TODO: add this to velocity in actor move function
}

XMFLOAT3 Physics::ApplyGravity_OLD( const ModelSpecs_W& worldSpecs, PhysAttributes& attributes, const float deltaT )
{
	// Given an initial position, velocity, and constant acceleration over a given 
	// time, use the displacement formula to calculate the new instantaneous position. 
	// *****Only acts on the Y component of the world position value of the object****

	// calculate incremental displacement
	XMFLOAT3 currPosition = worldSpecs.position;
	if ( attributes.velocLin.y < attributes.terminalV )
	{
		float deltaPosY = ( attributes.velocLin.y*deltaT ) + ( Gravity*( deltaT*deltaT ) ) * 0.5f;
		currPosition.y += deltaPosY;
	}
	else // Stop accelerating at terminal velocity
	{
		float deltaPosY = currPosition.y * deltaT;
		currPosition.y += deltaPosY;
	}
	// update velocity
	attributes.velocLin.y += Gravity * deltaT;

	return currPosition;
}
