#pragma once

#include "Actor.h"

class Actor_Player : public Actor
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

    void GetInput(const Input& pInput, int randI = 0.f, float randF = 0.f) override;

private:
	void Move( const DirectX::XMFLOAT3 &offset );


	void Rotate( const DirectX::XMFLOAT3 &rotation );
};