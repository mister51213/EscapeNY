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
    void GetInput(const Input& pInput, int randI=0.f, float randF=0.f) override
    {
        if (pInput.IsKeyDown(VK_RIGHT))
        {
            Move({ .6f, 0.f, 0.f });
        }
        if (pInput.IsKeyDown(VK_LEFT))
        {
            Move({ -.6f, 0.f, 0.f });
        }

        if (pInput.IsKeyDown(VK_UP))
        {
            Move({ 0.f, 0.f, .6f });
        }
        if (pInput.IsKeyDown(VK_DOWN))
        {
            Move({ 0.f, 0.f, -.6f });
        }

        m_worldSpecs.orientation.y += pInput.GetRelativeX();
		m_worldSpecs.orientation.x += pInput.GetRelativeY();
    } 

private:
    void Move(XMFLOAT3 offset) override
    { m_worldSpecs.position += offset; } 

    void Rotate(XMFLOAT3 rotation) override
    { m_worldSpecs.orientation += rotation; }

private:
    //std::shared_ptr<Input> m_pInput;
};