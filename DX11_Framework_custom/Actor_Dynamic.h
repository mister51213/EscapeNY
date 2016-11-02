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

	Actor_Dynamic();
	Actor_Dynamic( ModelSpecs_W worldSpecs, eTexture tex, ModelSpecs_L localSpecs, eModType modType );
	~Actor_Dynamic();
	void SetState( eActorState state );
	virtual void Update( float deltaT );
	void ConstantMove( const float deltaT );
	void ChaseTarget( const float deltaT );

	XMFLOAT3 MoveTowardTarget( const float deltaT );

protected: 
	eActorState m_state;
    XMFLOAT3 m_target = { 0.0f,0.0f,0.0f };
};

