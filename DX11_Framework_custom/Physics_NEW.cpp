#include "Physics_NEW.h"
#include "MathUtils.h"
#include "Utilities.h"
#include "Actor.h"

using namespace DirectX;

Physics_NEW::Physics_NEW()
{}

Physics_NEW::~Physics_NEW()
{}

void Physics_NEW::DoPhysics()
{}




void ForceEvent(PhysAttributes & attributes ) {}

void UpdateAcceleration(PhysAttributes& attributes ) {}

//TODO: Uupdating position will be handled locally by actor object.
void Physics_NEW::UpdatePosition( 
	ModelSpecs_W & specs, 
	PhysAttributes & attOld, // to get initial velocity
	PhysAttributes & attNew, // to get final velocity
	const float deltaT )
{
    XMFLOAT3 dispLinear = attOld.velocLin*deltaT + attNew.accelLin * deltaT*deltaT * 0.5;
	XMFLOAT3 newPosition = specs.position + dispLinear;
	specs.position = newPosition;
}

void Physics_NEW::UpdateOrientation( 
	ModelSpecs_W & specs, 
	PhysAttributes & attOld, // to get initial velocity
	PhysAttributes & attNew, // to get final velocity
	const float deltaT )
{
	XMFLOAT3 dispAngular = attOld.velocAng*deltaT + attNew.accelAng * deltaT*deltaT * 0.5;
	XMFLOAT3 newOrientation = specs.orientation + dispAngular;
	specs.orientation = newOrientation;
}









// reuse this, make all other collision physics from scratch
bool Physics_NEW::Overlaps(Actor* pActor1, Actor* pActor2)
{
	float collisionDist = pActor1->GetAttributes().radius +
		pActor2->GetAttributes().radius;

	XMFLOAT3 distVector = pActor1->GetWorldSpecs().position -
		pActor2->GetWorldSpecs().position;
	float actorDistance = Magnitude( distVector );

	if ( abs( actorDistance ) <= collisionDist )
	{
		return true;
	}
	else
		return false;
}
