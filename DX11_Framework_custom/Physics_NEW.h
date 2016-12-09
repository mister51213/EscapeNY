#pragma once

#include "Includes.h"

// FWD DECLARATIONS
struct AlignedAxisBoundingBox;
struct ModelSpecs_W;
struct PhysAttributes;
class Actor;
class Actor_Dynamic;

class Physics_NEW
{
public:
	Physics_NEW();
	~Physics_NEW();

	void DoPhysics();

	void Force_Steady( Actor_Dynamic* pActor, DirectX::XMFLOAT3 force, float deltaT);

	void Force_Collision( Actor_Dynamic * pActor, DirectX::XMFLOAT3 force );

	void AddDrag( Actor_Dynamic * pActor, float deltaT );










	// CONSERVATIVE - Gives back work
	/*
	gravity
	electric
	elastic

	KINETIC ENERGY = 0.5 * mass * velocity^2
	CHANGE in Kinetic E = Net work
	POTENTIAL ENERGY = -(work done by conservative forces)
	change in Kinetic E = -(change in Potential E)
	deltaK = - deltaU
	*/

	PhysAttributes Force_Conservative(PhysAttributes& partner);

	// NONCONSERVATIVE - Doesn't give back work
	/*
	friction
	drag
	*/
	PhysAttributes Force_NonConservative(PhysAttributes& partner);

	// Center of mass / Particle rotation
	/*
	divide each object into centers of mass
	to get realistic rotation about key centers of rotation
	*A system responds to an external force as if it were
	a SINGLE PARTICLE located at its center of mass.
	*/

};

