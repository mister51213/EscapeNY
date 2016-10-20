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

	XMFLOAT3 MoveToTarget();

	XMFLOAT3 Gravity( const float deltaT );
};

