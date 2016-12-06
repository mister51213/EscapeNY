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
	Actor_Dynamic( 
		const ModelSpecs_W &worldSpecs, 
		eTexture tex, 
		const ModelSpecs_L &localSpecs, 
		eModType modType );
	~Actor_Dynamic() = default;

	void SetState( eActorState state );
	eActorState GetState()const;

	void ResetPIDParams(const DirectX::XMFLOAT3 &target = { 0.f, 0.f, 0.f });

	virtual void Update( float deltaT );
	void UpdatePosition( const float deltaT );
	void ChaseTarget( const float deltaT );
	void Rebound_WRONG();
	void Rebound( Actor_Dynamic * partnerBall = nullptr );
	DirectX::XMFLOAT3 GetReboundFORCE( Actor_Dynamic * partnerBall );
	void ReboundDP( Actor_Dynamic* partnerBall = nullptr );
	void Stop();
	void PauseCollisionChecking();
	bool CheckMobility();
	void ResumeCollisionChecking();
	bool CollisionTurnedOff();
	DirectX::XMFLOAT3 GetInitialPosition() const;

protected: 
	float integrator;

	eActorState m_state;
	DirectX::XMFLOAT3 m_target = { 0.0f,0.0f,0.0f };

	DirectX::XMFLOAT3 m_initalPosition = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 m_initialHeading = { 0.0f, 0.0f, 0.0f };
	float m_halfway = 0.0f;
	float m_integrator = 0.0f;

	bool m_stopCheckFlag = false;

	bool m_isMovable;
};


