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
void Physics_NEW::Force_Steady( Actor_Dynamic* pActor, DirectX::XMFLOAT3 force, float deltaT)
{
	DirectX::XMFLOAT3 accelApplied = force / pActor->m_attributes.mass; // a = f / m
	pActor->m_attributes.accelLin += accelApplied * deltaT; 
}
//TODO: this isnt how forces work!
// It shouldnt increase acceleration and then just leave it there.
// acceleration should go back to zero after force subsides.

// Note: we treat collisions as an INSTANTANEOUS change in velocity; therefore we dont 
// even change the actor's acceleration and do a one time change in the actor's velocity only.
// - saves us the burden of setting acceleration back to 0 when force is spent
void Physics_NEW::Force_Instantaneous( Actor_Dynamic* pActor, DirectX::XMFLOAT3 impulse)
{
	pActor->m_attributes.velocLin += impulse; 
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
		
		Force_Steady( pActor, dragForce, deltaT );

	}
}