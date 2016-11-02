#pragma once

#include "Includes.h"
#include "Utilities.h"
#include "MathUtils.h"

class Physics
{
public:
	// TODO: Can change these later if default not needed
	Physics() = default;
	~Physics() = default;

	///////////////////// GENERAL ///////////////////////
	ModelSpecs_W UpdateAll(PhysAttributes& attributes);
	
	/////// NEW MOTION /////////
	DirectX::XMFLOAT3 MoveToTarget_ALT( const ModelSpecs_W & worldSpecs, PhysAttributes& attributes, const float deltaT ) const;

	/////////////// COLLISION RELATED ///////////////////

	// AlignedAxisBoundingBox doesn't care what shape the mesh is, it's a BOX.
	// If you care what the model type is, you need to make different Bounding
	// objects, like BoundingTriangle, BoundingFrustum, BoundingSphere or BoundingMesh.
	AlignedAxisBoundingBox MakeBoundingBox( const std::vector<DirectX::XMFLOAT3> &VertexList);

	bool CheckCollision( 
		const XMFLOAT3 &PositionA, const AlignedAxisBoundingBox &BoundsA, 
		const XMFLOAT3 &PositionB, const AlignedAxisBoundingBox &BoundsB );

	bool CorrectCollision();

	DirectX::XMFLOAT3 ApplyGravity_ALT( const float Mass, const float DeltaTime );

	DirectX::XMFLOAT3 ApplyWind( const float Mass, const float DeltaTime );

	void DoPhysics( PhysAttributes& attributes, const float DeltaTime );

	/////////////// MOTION RELATED ///////////////////

	DirectX::XMFLOAT3 MoveToTarget( const ModelSpecs_W & worldSpecs, PhysAttributes& attributes, const float deltaT );

	DirectX::XMFLOAT3 MoveTowardTarget( const ModelSpecs_W& worldSpecs, PhysAttributes& attributes, const float deltaT );

	DirectX::XMFLOAT3 ApplyGravity( const ModelSpecs_W& worldSpecs, PhysAttributes& attributes, const float deltaT );

	DirectX::XMFLOAT3 ApplyForce( eForceType forceType );

public:
	//PhysAttributes m_attributes;

private:
	BoundingMesh m_bounds;
};

