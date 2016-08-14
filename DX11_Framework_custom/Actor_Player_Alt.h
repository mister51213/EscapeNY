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

	void SetPosition( const DirectX::XMFLOAT3 &Position );
	void GetInput( const Input& pInput, int randI = 0.f, float randF = 0.f ) override;
	
	
private:
	float m_speed;
};

