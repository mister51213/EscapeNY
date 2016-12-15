#include "Actor_Dynamic.h"
#include "MathUtils.h"

using namespace DirectX;

Actor_Dynamic::Actor_Dynamic( const ModelSpecs_W &worldSpecs,
							  eTexture tex,
							  const ModelSpecs_L &localSpecs,
							  eModType modType )
	:
	Actor( worldSpecs, tex, localSpecs, modType )
{
	ResetPIDParams();
}

/// ACCESSOR FUNCTIONS ///
void Actor_Dynamic::SetState(eActorState state){ m_state = state;}
Actor_Dynamic::eActorState Actor_Dynamic::GetState() const{	return m_state;}
XMFLOAT3 Actor_Dynamic::GetInitialPosition() const{	return m_initalPosition;}
bool Actor_Dynamic::CheckMobility(){return m_isMovable;}
bool Actor_Dynamic::CollisionOn(){ return m_collisionOn;}

/// Reset / State functions ///
void Actor_Dynamic::Update(float deltaT)
{
        switch (m_state)
        {
        case STOPPED:
		{
			m_attributes.velocLin = { 0.f, 0.f, 0.f };
			return;
		}
        case FALLING:
            {}
        case HOMING:
            {
                ChaseTarget(deltaT); // TODO: change this to non-default parameter
				break;
            }
		case CONST_MOVE:
            {
                UpdateMotion(deltaT);
				break;
            }
        default:
            return;
        }
}
void Actor_Dynamic::ResetPIDParams( const XMFLOAT3 &target )
{
	m_target = target;
	m_initalPosition = GetPosition();
	m_initialHeading = m_target - m_initalPosition;
	m_halfway = abs(Magnitude(m_initialHeading)) * 0.5;
}

/// Motion functions ///
void Actor_Dynamic::UpdateMotion( const float deltaT )
{
	// Store a copy of position and attributes from last iteration for derivative calculations
	m_previousPosition = GetPosition();
	m_attributesPrev = m_attributes;

	/// UPDATE POSITION ///
	XMFLOAT3 dispLinear =
		m_attributesPrev.velocLin*deltaT +
		m_attributes.accelLin * deltaT*deltaT * 0.5;
	XMFLOAT3 avgAccel = ( m_attributesPrev.accelLin + m_attributes.accelLin ) *0.5;
	m_attributes.velocLin += avgAccel * deltaT;
	m_worldSpecs.position = m_worldSpecs.position + dispLinear;

	/// UPDATE ROTATION ///
	XMFLOAT3 dispAngular =
		m_attributesPrev.velocAng*deltaT +
		m_attributes.accelAng * deltaT*deltaT * 0.5;
	XMFLOAT3 avgAccAng = ( m_attributesPrev.accelAng + m_attributes.accelAng ) *0.5;
	m_attributes.velocAng += avgAccAng * deltaT;
	m_worldSpecs.orientation = m_worldSpecs.orientation + dispAngular;

	// Update bounding box according to actor position
	m_AABBox.ResetMinMax(m_worldSpecs.position);
}

void Actor_Dynamic::StopAccelerating( )
{
	m_attributes.accelLin = { 0.f, 0.f, 0.f };
}

void Actor_Dynamic::ChaseTarget(const float deltaT )
{
		// Store copy of attributes from last iteration for derivative calculations
		m_attributesPrev = m_attributes;

		XMFLOAT3 currDistV = m_target - m_worldSpecs.position;
		float error = Magnitude( currDistV );
		XMFLOAT3 dirToTarget = Normalize(currDistV);
		
		float pCoeff = 0.2f; // proportional gain coefficient
		float dCoeff = -.07f; // derivative gain coefficient
		m_integrator += error; // TODO: add conditional statement to apply this
		/*
		NOTE: Integrator is only needed if real world friction applies.
		In other words, if the thrust input doesn't produce the velocity
		that you expect, because some of it is lost to friction, then you
		have to creep the rest of the way to the goal using the integrator.

		So to add another level of realism, this function shouldnt be able to
		automatically increment acceleration and velocity, but rather input a force, 
		and the result of that force will be affected by other forces in the world, 
		such as friction and drag. Then it will check its own resultant velocity, 
		and continue to calculate	how much input it needs to give based on that. 
		
		This is how PID works in the real world.
		*/
		m_attributes.accelLin = dirToTarget * error * pCoeff;
		m_attributes.accelLin += m_attributes.velocLin * dCoeff;
		// TODO: Add friction to damp the speed as it approaches target;
		// TODO: that way, we won't only rely on P and D coefficients

		////////// UNIVERSAL FINAL CALCULATION ///////
		m_attributes.velocLin += m_attributes.accelLin;
		XMFLOAT3 displacement = m_attributes.velocLin * deltaT;
		m_worldSpecs.position += displacement;
}
void Actor_Dynamic::Stop(){	
	m_attributes.accelLin = {0.f,0.f,0.f};
	m_attributes.velocLin = {0.f,0.f,0.f};
	// TODO: make a function in the attributes struct to zero our everything
}

/// Collision related functions ///
// NOTE: currently only works with another ball
void Actor_Dynamic::ReboundWith(std::vector<Actor_Dynamic>::iterator& partner)
{
	PhysAttributes& partnerAttributes = partner->GetAttributes();
	// calculate velocity change for this actor
	float momentumTransferRatio = partnerAttributes.mass / m_attributes.mass;
	XMFLOAT3 forceOnThis;
	XMFLOAT3 forceOnPartner;

	if ( partnerAttributes.mass >= 4000 ) // INFINITE MASS
	{
		XMFLOAT3 rbndComponent = {0.f, -m_attributes.velocLin.y, 0.f};
		rbndComponent *= 2.0f;

		forceOnThis = m_attributes.velocLin + rbndComponent;
		forceOnPartner = { 0.f, 0.f, 0.f };
		
		m_attributes.velocLin.y = 0.f; // TODO: component perpendicular to trajectory only
	}
	else if ( m_attributes.mass >= 4000 ) // INFINITE MASS
	{
		XMFLOAT3 rbndComponent = {0.f, -partnerAttributes.velocLin.y, 0.f};
		rbndComponent *= 2.0f;

		forceOnThis = { 0.f, 0.f, 0.f };
		forceOnPartner = partnerAttributes.velocLin + rbndComponent;

		/* 
		calculate force component perpendicular to the surface of collision
		1. Get the normal value from that model (or tangent and binormal)
		2. use DP to PROJECT the current velocity vector onto that normal vector
		3. return that DP component * -2 to be added to the current velocity
		*/

		partner->m_attributes.velocLin.y = 0.f; // TODO: component perpendicular to trajectory only
	}

	else // FINITE MASS
	{
		forceOnThis = partnerAttributes.velocLin * momentumTransferRatio;
		forceOnPartner = m_attributes.velocLin / momentumTransferRatio;
		m_attributes.velocLin = { 0.f, 0.f, 0.f };
		partner->m_attributes.velocLin = { 0.f, 0.f, 0.f };
	}
	
	m_attributes.velocLin += forceOnThis;
	partner->m_attributes.velocLin += forceOnPartner;
}

void Actor_Dynamic::ReboundX1()
{
	// calculate velocity change for this actor
	m_attributes.velocLin.y *= -1.0f;

	PauseCollision();
}

void Actor_Dynamic::Rebound(Actor_Dynamic* partnerBall)
{
	// TODO: try replacing current position with target and seeing if it works better
	// NOTE: initialPosition is added, then subtracted, so they cancel out, but
	// left them here commented out just to show how the vectors are arrived at visually.
	XMFLOAT3 thisTrajectory = m_worldSpecs.position/* - m_initalPosition*/;
	XMFLOAT3 otherTrajectory = partnerBall->GetPosition() - partnerBall->GetInitialPosition();
	m_target = thisTrajectory + otherTrajectory/* + m_initalPosition*/;
	//TODO: cam't use target anymore, have to apply force on other ball

	PauseCollision();
}

void Actor_Dynamic::ResumeCollision(){m_collisionOn = true;}
void Actor_Dynamic::PauseCollision(){m_collisionOn = false;}
