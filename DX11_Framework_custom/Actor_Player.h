#pragma once

#include "Actor_Dynamic.h"


class Actor_Player : public Actor_Dynamic
{
public:
	Actor_Player() = default;

	// Enums are the same size as references and pointers so we can just
	// pass them by value
	Actor_Player(
		const ModelSpecs_W & worldSpecs, 
		eTexture tex, 
		const ModelSpecs_L & localSpecs, 
		eModType modType = CUBE_TEXTURED );

    void GetInput( Input& pInput, int randI = 0.f, float randF = 0.f ) override;

private:
	bool leftPressLastFrame = false;
	bool rightPressLastFrame = false;
	bool upPressLastFrame = false;
	bool downPressLastFrame = false;
};