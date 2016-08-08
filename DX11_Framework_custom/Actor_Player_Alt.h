#pragma once

#include "Actor.h"
#include "Input.h"


class Actor_Player_Alt:
	public Actor
{
public:
	Actor_Player_Alt();
	Actor_Player_Alt(
		const ModelSpecs_W &worldSpecs,
		eTexture tex,
		const ModelSpecs_L &localSpecs,
		eModType modType = CUBE_TEXTURED );
	~Actor_Player_Alt();

	void Update( const Input &UserInput);
	
private:
	float m_speed;
};

