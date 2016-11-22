#include "Actor_Dynamic.h"



Actor_Dynamic::Actor_Dynamic(ModelSpecs_W worldSpecs, eTexture tex, ModelSpecs_L localSpecs, eModType modType): 
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

void Actor_Dynamic::ResetPIDParams( XMFLOAT3 target )
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
			m_attributes.velocity = { 0.f, 0.f, 0.f };
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
                ConstantMove(deltaT);
				break;
            }
        default:
            return;
        }
}

void Actor_Dynamic::ConstantMove( const float deltaT)
{
	XMFLOAT3 deltaPos = m_attributes.velocity * deltaT;
    m_worldSpecs.position += deltaPos;
}

void Actor_Dynamic::ChaseTarget(const float deltaT )
{
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
		m_attributes.acceleration = dirToTarget * error * pCoeff;
		m_attributes.acceleration += m_attributes.velocity * dCoeff;
		// TODO: Add friction to damp the speed as it approaches target;
		// TODO: that way, we won't only rely on P and D coefficients

		////////// UNIVERSAL FINAL CALCULATION ///////
		m_attributes.velocity += m_attributes.acceleration;
		XMFLOAT3 displacement = m_attributes.velocity * deltaT;
		m_worldSpecs.position += displacement;
}

void Actor_Dynamic::Rebound()
{
	XMFLOAT3 currVeloc = Normalize( m_attributes.velocity );
	XMFLOAT3 reverseDir = -currVeloc;
	XMFLOAT3 newTarget = reverseDir * 300.f;
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

void Actor_Dynamic::PauseCollisionChecking()
{
	m_stopCheckFlag = true;
}
