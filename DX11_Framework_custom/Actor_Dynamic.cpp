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

void Actor_Dynamic::SetState(eActorState state)
{
	 m_state = state;
}

Actor_Dynamic::eActorState Actor_Dynamic::GetState() const
{
	return m_state;
}

void Actor_Dynamic::ResetPIDParams( const XMFLOAT3 &target )
{
	m_target = target;
	m_initalPosition = GetPosition();
	m_initialHeading = m_target - m_initalPosition;
	m_halfway = abs(Magnitude(m_initialHeading)) * 0.5;
}

void Actor_Dynamic::Update(float deltaT)
{
        switch (m_state)
        {
        case STILL:
		{
			m_attributes.velocLin = { 0.f, 0.f, 0.f };
			return;
		}
        case FALLING:
            {

            }
        case HOMING:
            {
                ChaseTarget(deltaT); // TODO: change this to non-default parameter
				break;
            }
		case CONST_MOVE:
            {
                UpdatePosition(deltaT);
				break;
            }
        default:
            return;
        }
}

void Actor_Dynamic::UpdatePosition( const float deltaT)
{
    XMFLOAT3 deltaPos = m_attributes.velocLin * deltaT;
	m_worldSpecs.position += deltaPos;
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

//TODO:
/*
Now, since it only negates their velocties on rebound, 
the rebound physics are not quite accurate. 
We need to properly reflect these vectors across the plane 
that is perpendicular to the balls respective flight paths instead.
*/

void Actor_Dynamic::Rebound_WRONG()
{
	float reboundMagnitude = Magnitude( m_attributes.velocLin );
	XMFLOAT3 currVeloc = Normalize( m_attributes.velocLin );
	XMFLOAT3 reverseDir = -currVeloc;
	XMFLOAT3 newTarget = reverseDir * reboundMagnitude;
	m_target = newTarget;
	PauseCollisionChecking();
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

	PauseCollisionChecking();
}

XMFLOAT3 Actor_Dynamic::GetReboundFORCE(Actor_Dynamic* partnerBall)
{
	// TODO: try replacing current position with target and seeing if it works better
	// NOTE: initialPosition is added, then subtracted, so they cancel out, but
	// left them here commented out just to show how the vectors are arrived at visually.
	XMFLOAT3 thisTrajectory = m_worldSpecs.position/* - m_initalPosition*/;
	XMFLOAT3 otherTrajectory = partnerBall->GetPosition() - partnerBall->GetInitialPosition();
	XMFLOAT3 force = thisTrajectory + otherTrajectory/* + m_initalPosition*/;

	PauseCollisionChecking();

	return force;
}

void Actor_Dynamic::ReboundDP(Actor_Dynamic* partnerBall)
{
	float reboundMagnitude = Magnitude( m_attributes.velocLin );
	XMFLOAT3 veloc1 = Normalize( m_attributes.velocLin );
	XMFLOAT3 veloc2 = Normalize( partnerBall->GetAttributes().velocLin );

	//float DP = DotProduct( veloc2, veloc1 );
	//TODO: 
	/*
	1. take dot product between two balls' velocities (normalized)
	2. maintain the Y component of each of their velocities
	3. flip the X component of each of their velocities
	4. scale the resultant vector by the dot product	
	*/

	// simply swap velocities here
	XMFLOAT3 reverseDir = veloc2/* * DP */;
	XMFLOAT3 newTarget = reverseDir * reboundMagnitude;

	//TODO:
	/*
	negate only the perpendicular component of the vector of approach;
	get that component efficiently using dot product

	U [dot] V = 
	U.x*V.x + U.y*V.y = 
	magnitude(U)*magnitude(V)*cos(angle between 'em)
	
	*/
		
	m_target = newTarget;

	PauseCollisionChecking();
}

void Actor_Dynamic::Stop()
{
	m_state = STILL;
}

void Actor_Dynamic::ResumeCollisionChecking()
{
	m_stopCheckFlag = false;
}

bool Actor_Dynamic::CollisionTurnedOff()
{
	return m_stopCheckFlag;
}

XMFLOAT3 Actor_Dynamic::GetInitialPosition() const
{
	return m_initalPosition;
}

void Actor_Dynamic::PauseCollisionChecking()
{
	m_stopCheckFlag = true;
}

bool Actor_Dynamic::CheckMobility()
{
	return m_isMovable;
}