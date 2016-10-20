#pragma once

#include "Includes.h"
#include "Utilities.h"

class Physics
{
public:
	Physics();
	~Physics();

	bool CheckCollision();

	bool CorrectCollision();

	XMFLOAT3 MoveTowardTarget( const ModelSpecs_W& worldSpecs, float deltaT );

	XMFLOAT3 ApplyGravity( const ModelSpecs_W& worldSpecs, const float deltaT );

	XMFLOAT3 ApplyForce(eForceType forceType);

public:
	PhysAttributes m_attributes;
};

