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

// TODO: Add force (impulse) function
void Physics_NEW::GenericForce( Actor_Dynamic* pActor, DirectX::XMFLOAT3 force, float deltaT)
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
		
		GenericForce( pActor, dragForce, deltaT );

	}
}