#pragma once

#include "Includes.h"

// Since the Actors would be the ones with the bounding objects, 
// everything in this header can be forward declared.  If you add any
// objects to this header that aren't references or pointers, you must
// add the header that it belongs to, if in Utilities.h for instance, 
// add Utilities.h.  If you do add Utilities.h, remove the forward 
// declarations that are associated with that file.  

// Moved #include Utilites.h to Physics.cpp
struct AlignedAxisBoundingBox;
struct ModelSpecs_W;
struct PhysAttributes;

class Physics
{
public:
	///////////////////// GENERAL ///////////////////////
	//ModelSpecs_W UpdateAll(PhysAttributes& attributes);
	void ApplyImpulse(PhysAttributes& attributes, const float Mass, const float DeltaTime );

	/////////////// COLLISION RELATED ///////////////////

	// AlignedAxisBoundingBox doesn't care what shape the mesh is, it's a BOX.
	// If you care what the model type is, you need to make different Bounding
	// objects, like BoundingTriangle, BoundingFrustum, BoundingSphere or BoundingMesh.
	AlignedAxisBoundingBox MakeBoundingBox( const std::vector<DirectX::XMFLOAT3> &VertexList);

	bool CheckCollision( 
		const DirectX::XMFLOAT3 &PositionA, const AlignedAxisBoundingBox &BoundsA,
		const DirectX::XMFLOAT3 &PositionB, const AlignedAxisBoundingBox &BoundsB );

	bool CorrectCollision();

	/////////////// FORCE RELATED ///////////////////
	DirectX::XMFLOAT3 ApplyGravity( const float Mass, const float DeltaTime );
	DirectX::XMFLOAT3 ApplyGravity_OLD( const ModelSpecs_W& worldSpecs, PhysAttributes& attributes, const float deltaT );
	DirectX::XMFLOAT3 ApplyWind( const float Mass, const float DeltaTime );

	void DoPhysics( PhysAttributes& attributes, const float DeltaTime );

private:
	//BoundingMesh m_bounds;
};

