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
        eModType modType = CUBE_TEXTURED)/* :
        Actor(worldSpecs, tex, localSpecs, modType = CUBE_TEXTURED)*/ // base constructor
    { 
        //m_pInput = pInput;
        m_worldSpecs = worldSpecs;
        m_localSpecs = localSpecs;
        m_texIndex = tex;
        m_modType = modType;
    }

public:
    void GetInput(std::shared_ptr<Input> pInput) override
    {
        if (pInput->IsKeyDown(VK_RIGHT))
        {
            Move({ .6f, 0.f, 0.f });
        }
        else if (pInput->IsKeyDown(VK_LEFT))
        {
            Move({ -.6f, 0.f, 0.f });
        }

        if (pInput->IsKeyDown(VK_UP))
        {
            Move({ 0.f, 0.f, .6f });
        }
        else if (pInput->IsKeyDown(VK_DOWN))
        {
            Move({ 0.f, 0.f, -.6f });
        }
    } 

private:
    void Move(XMFLOAT3 offset) override
    { m_worldSpecs.position += offset; } 

    void Rotate(XMFLOAT3 rotation) override
    { m_worldSpecs.orientation += rotation; }

private:
    //std::shared_ptr<Input> m_pInput;
};