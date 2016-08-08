#pragma once
#include "Actor.h"

class Actor_Player : public Actor
{
public:
    Actor_Player() {}

    Actor_Player(
        ModelSpecs_W worldSpecs,
        eTexture tex,
        ModelSpecs_L localSpecs,
        eModType modType = CUBE_TEXTURED) :
        Actor(worldSpecs, tex, localSpecs, modType = CUBE_TEXTURED) // base constructor
    { }

public:
    void GetInput(std::shared_ptr<Input> pInput, int randI=0.f, float randF=0.f) override
    {
        if (pInput->IsKeyDown(VK_RIGHT))
        {
            Move({ .6f, 0.f, 0.f });
        }
        if (pInput->IsKeyDown(VK_LEFT))
        {
            Move({ -.6f, 0.f, 0.f });
        }

        if (pInput->IsKeyDown(VK_UP))
        {
            Move({ 0.f, 0.f, .6f });
        }
        if (pInput->IsKeyDown(VK_DOWN))
        {
            Move({ 0.f, 0.f, -.6f });
        }

        // TEST: ShapeBuilder.cpp line 171~
        m_worldSpecs.orientation = pInput->m_LastMousePos;
    } 

private:
    void Move(XMFLOAT3 offset) override
    { m_worldSpecs.position += offset; } 

    void Rotate(XMFLOAT3 rotation) override
    { m_worldSpecs.orientation += rotation; }

private:
    //std::shared_ptr<Input> m_pInput;
};