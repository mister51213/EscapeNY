#pragma once

#include "Actor.h"

class Actor_NPC : public Actor
{
public:
	Actor_NPC() = default;

	Actor_NPC(
		const ModelSpecs_W &worldSpecs,
		eTexture tex,
		const ModelSpecs_L &localSpecs,
		eModType mod = CUBE_TEXTURED );

public:
	// Leaving these functions here for you to test with, 
	// please remember to put in CPP files when done.
	void GetInput( const Input& pInput, int randI, float randF ) override;

private:
	void Move( const DirectX::XMFLOAT3 &offset );
	void Rotate( const DirectX::XMFLOAT3 &rotation );
};
