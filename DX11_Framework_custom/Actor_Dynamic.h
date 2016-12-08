#pragma once

#include "Actor.h"

class Actor_Dynamic :
	public Actor
{
public:
	enum eActorState
	{	
		// TODO: REMOVE THESE, add specific state collections for each Actor child
		STOPPED, FALLING, CONST_MOVE, HOMING,
		PLAYER_IDLE, PLAYER_WALKING, PLAYER_PUSHING, PLAYER_QUICKTIME, PLAYER_DROWNING, PLAYER_PINNED
	};

	Actor_Dynamic() = default;
	Actor_Dynamic( 
		const ModelSpecs_W &worldSpecs, 
		eTexture tex, 
		const ModelSpecs_L &localSpecs, 
		eModType modType );
	~Actor_Dynamic() = default;

	/// ACCESSOR FUNCTIONS ///
	void SetState( eActorState state );
	eActorState GetState()const;
	bool CheckMobility();
	DirectX::XMFLOAT3 GetInitialPosition() const;
	
	/// Reset / update functions ///
	void ResetPIDParams(const DirectX::XMFLOAT3 &target = { 0.f, 0.f, 0.f });
	virtual void Update( float deltaT );

	/// Motion functions ///
	void UpdateMotion( const float deltaT );
	void StopAccelerating();
	void ChaseTarget( const float deltaT );
	void Stop();

	/// Collision related functions ///
	void Rebound( Actor_Dynamic * partnerBall );
	DirectX::XMFLOAT3 GetReboundForce( Actor_Dynamic * partnerBall );
	void PauseCollisionChecking();
	void ResumeCollisionChecking();
	bool CollisionOn();

protected: 
	float integrator;

	eActorState m_state;
	DirectX::XMFLOAT3 m_target = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 m_initalPosition = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 m_initialHeading = { 0.0f, 0.0f, 0.0f };
	float m_halfway = 0.0f;
	float m_integrator = 0.0f;

	bool m_collisionOn = true;

	bool m_isMovable;

	DirectX::XMFLOAT3 m_previousPosition = { 0.0f, 0.0f, 0.0f };
};


