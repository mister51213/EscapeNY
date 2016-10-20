#include "Physics.h"

Physics::Physics()
{}

Physics::~Physics()
{}

bool Physics::CheckCollision()
{
	return false;
}

bool Physics::CorrectCollision()
{
	return false;
}

XMFLOAT3 Physics::MoveToTarget()
{
	return XMFLOAT3();
}

XMFLOAT3 Physics::Gravity( const float deltaT )
{
	return XMFLOAT3();
}

