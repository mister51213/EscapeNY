#pragma once
#include "Actor.h"

class Actor_Dynamic :
	public Actor
{
public:
	enum eActorState
	{	
		// TODO: REMOVE THESE, add specific state collections for each Actor child
		Stationary, Falling, Moving, Move_PID,

		PLAYER_IDLE, PLAYER_WALKING, PLAYER_PUSHING, PLAYER_QUICKTIME, PLAYER_DROWNING, PLAYER_PINNED
	};

	Actor_Dynamic();
	Actor_Dynamic( ModelSpecs_W worldSpecs, eTexture tex, ModelSpecs_L localSpecs, eModType modType );
	~Actor_Dynamic();
	void SetState( eActorState state );
	virtual void Update( float deltaT );
	void Move( const float deltaT );
	void MovePID( float deltaT );
	void MoveToTarget_ALT2( const float deltaT );

protected: 
	eActorState m_state;
};

