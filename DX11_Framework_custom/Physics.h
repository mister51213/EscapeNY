#pragma once

#include "Includes.h"
#include "Utilities.h"

class Physics
{
public:
	// TODO: Can change these later if default not needed
	Physics() = default;
	~Physics() = default;

	bool CheckCollision(BoundingBox bounds );

	bool CorrectCollision();

	DirectX::XMFLOAT3 MoveToTarget( const ModelSpecs_W & worldSpecs, const float deltaT );

	DirectX::XMFLOAT3 MoveTowardTarget( const ModelSpecs_W& worldSpecs, const float deltaT );

	DirectX::XMFLOAT3 ApplyGravity( const ModelSpecs_W& worldSpecs, const float deltaT );

	DirectX::XMFLOAT3 ApplyForce( eForceType forceType );

public:
	PhysAttributes m_attributes;
};

