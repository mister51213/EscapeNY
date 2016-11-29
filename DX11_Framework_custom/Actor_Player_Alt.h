#pragma once

#include "Actor_Dynamic.h"
#include "Utilities.h"

class Actor_Player_Alt:
	public Actor_Dynamic
{
public:
	Actor_Player_Alt() = default;
	Actor_Player_Alt(
		const ModelSpecs_W &worldSpecs,
		eTexture tex,
		Input *pInput,
		eModType modType = CUBE_TEXTURED );
	~Actor_Player_Alt() = default;

	void Update( float deltaT )override final;
	
private:
	void Walk( float DeltaTime );
	void Push( float DeltaTime );
	void QuickTime();
private:
	AlignedAxisBoundingBox m_boundingbox;
	Input *m_pInput;
	float m_speed = .2f;
};

