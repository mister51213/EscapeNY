#include "Physics_NEW.h"
#include "MathUtils.h"
#include "Utilities.h"
#include "Actor_Dynamic.h"

using namespace DirectX;

Physics_NEW::Physics_NEW()
{}

Physics_NEW::~Physics_NEW()
{}

void Physics_NEW::DoPhysics()
{}


void ForceEvent(PhysAttributes & attributes ) {}

void UpdateAcceleration(PhysAttributes& attributes ) {}

/*
NOTE: 
PhysAttributes struct should be made more smart and have its 
own functions to calculate its momentum on update, etc.
*/

void Physics_NEW::UpdateActor( Actor_Dynamic* pActor, const float deltaT )
{
	// Store a copy of position and attributes from last iteration for derivative calculations
	pActor->m_previousPosition = pActor->GetPosition();
	pActor->m_attributesPrev = pActor->m_attributes;
	ModelSpecs_W specs = pActor->GetWorldSpecs();
	//TODO: make this happen automatically within the physAttributes struct

	/// UPDATE POSITION ///
    XMFLOAT3 dispLinear = 
		pActor->m_attributesPrev.velocLin*deltaT + 
		pActor->m_attributes.accelLin * deltaT*deltaT * 0.5;
	XMFLOAT3 avgAccel = ( pActor->m_attributesPrev.accelLin + pActor->m_attributes.accelLin ) *0.5;
	pActor->m_attributes.velocLin += avgAccel * deltaT;
	specs.position = specs.position + dispLinear;

	/// UPDATE ROTATION ///
	XMFLOAT3 dispAngular = 
		pActor->m_attributesPrev.velocAng*deltaT + 
		pActor->m_attributes.accelAng * deltaT*deltaT * 0.5;
	XMFLOAT3 avgAccAng = ( pActor->m_attributesPrev.accelAng + pActor->m_attributes.accelAng ) *0.5;
	pActor->m_attributes.velocAng += avgAccAng * deltaT;
	specs.orientation = specs.orientation + dispAngular;

	/// GIVE ACTOR UPDATED SPECS ///
	pActor->SetWorldSpecs( specs );
}

// TODO: Add force (impulse) function
void Physics_NEW::ApplyForce( Actor_Dynamic* pActor, DirectX::XMFLOAT3 force, float deltaT)
{
	DirectX::XMFLOAT3 accelApplied = force / pActor->m_attributes.mass; // a = f / m
	pActor->m_attributes.accelLin += accelApplied * deltaT; 
}

void Physics_NEW::AddDrag( Actor_Dynamic* pActor, float deltaT)
{
	XMFLOAT3 actorVeloc = pActor->m_attributes.velocLin;
	float veloMag = Magnitude( actorVeloc );

	if ( veloMag != 0.f )
	{
		float dragCoef = .1f; // note: fulid density and area discounted
		
		XMFLOAT3 dragForce = actorVeloc * veloMag * dragCoef; // calculate
		dragForce = -dragForce; // negate
		
		ApplyForce( pActor, dragForce, deltaT );

	}
}



// TODO: Add collision function

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
	//TODO: optimize this by comparing a^2 + b^2 = c^2 and not calculating distance

	if ( abs( actorDistance ) <= collisionDist )
	{
		return true;
	}
	else
		return false;
}
