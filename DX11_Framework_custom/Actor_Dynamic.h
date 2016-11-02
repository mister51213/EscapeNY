#pragma once
#include "Actor.h"

class Actor_Dynamic :
	public Actor
{
public:
	Actor_Dynamic();
	Actor_Dynamic( ModelSpecs_W worldSpecs, eTexture tex, ModelSpecs_L localSpecs, eModType modType );
	~Actor_Dynamic();
	void Move( const float deltaT );
	void MovePID( float deltaT );
	void MoveToTarget_ALT2( const float deltaT );
};

