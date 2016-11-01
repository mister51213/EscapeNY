#pragma once

#include "Includes.h"
#include "Utilities.h"

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

	void MakeBoundingBox(BoundingBox bounds, eModType type = SPHERE);

	bool CheckCollision(BoundingBox bounds );

	bool CorrectCollision();

	/////////////// MOTION RELATED ///////////////////

	DirectX::XMFLOAT3 MoveToTarget( const ModelSpecs_W & worldSpecs, PhysAttributes& attributes, const float deltaT );

	DirectX::XMFLOAT3 MoveTowardTarget( const ModelSpecs_W& worldSpecs, PhysAttributes& attributes, const float deltaT );

	DirectX::XMFLOAT3 ApplyGravity( const ModelSpecs_W& worldSpecs, PhysAttributes& attributes, const float deltaT );

	DirectX::XMFLOAT3 ApplyForce( eForceType forceType );

public:
	//PhysAttributes m_attributes;

private:
	BoundingBox m_bounds;
};

