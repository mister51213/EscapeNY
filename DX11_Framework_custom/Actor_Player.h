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


public:
	// Left this stuff here in case you are still testing it, just 
	// try to remember to move to CPP file when you are done testing
    void GetInput(const Input& pInput, int randI = 0.f, float randF = 0.f) override;

private:
	void Move( const XMFLOAT3 &offset )
	{ m_worldSpecs.position += offset; }

	void Rotate( const XMFLOAT3 &rotation )
	{ m_worldSpecs.orientation += rotation; }

private:
    //std::shared_ptr<Input> m_pInput;
};