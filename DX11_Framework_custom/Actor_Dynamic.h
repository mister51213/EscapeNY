#pragma once
#include "Actor.h"

class Actor_Dynamic :
	public Actor
{
public:
	enum eActorState
	{	
		// TODO: REMOVE THESE, add specific state collections for each Actor child
		STILL, FALLING, CONST_MOVE, HOMING,
		PLAYER_IDLE, PLAYER_WALKING, PLAYER_PUSHING, PLAYER_QUICKTIME, PLAYER_DROWNING, PLAYER_PINNED
	};

	Actor_Dynamic() = default;
	Actor_Dynamic( ModelSpecs_W worldSpecs, eTexture tex, ModelSpecs_L localSpecs, eModType modType );
	~Actor_Dynamic() = default;

	void SetState( eActorState state );
	eActorState GetState()const;

	void ResetPIDParams(XMFLOAT3 target = { 0.f, 0.f, 0.f });

	virtual void Update( float deltaT );
	void ConstantMove( const float deltaT );
	void ChaseTarget( const float deltaT );
	void Rebound_WRONG();
	void Rebound( Actor_Dynamic * partnerBall );
	void ReboundDP(Actor_Dynamic* partnerBall);
	void Stop();
	void PauseCollisionChecking();
	void ResumeCollisionChecking();
	bool CollisionTurnedOff();
	XMFLOAT3 GetInitialPosition() const;

protected: 
	float integrator;

	eActorState m_state;
    XMFLOAT3 m_target = { 0.0f,0.0f,0.0f };

	XMFLOAT3 m_initalPosition = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 m_initialHeading = { 0.0f, 0.0f, 0.0f };
	float m_halfway = 0.0f;
	float m_integrator = 0.0f;

	bool m_stopCheckFlag = false;
};


