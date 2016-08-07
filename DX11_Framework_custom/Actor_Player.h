#pragma once
#include "Actor.h"
#include "Input.h"

class Actor_Player : public Actor
{
public:
    Actor_Player() {}

    Actor_Player(
        //std::shared_ptr<Input> pInput,
        ModelSpecs_W worldSpecs,
        eTexture tex,
        ModelSpecs_L localSpecs,
        eModType modType = CUBE_TEXTURED) :
        Actor(worldSpecs, tex, localSpecs, modType = CUBE_TEXTURED) // base constructor
    { 
        //m_pInput = pInput;
        m_worldSpecs = worldSpecs;
        m_localSpecs = localSpecs;
        m_texIndex = tex;
        m_modType = modType;
    }

    // copy constructor
    Actor_Player operator=(const Actor_Player& player)
    {
        Actor_Player newPlayer;
        //newPlayer.m_pInput = player.m_pInput;
        newPlayer.m_worldSpecs = player.m_worldSpecs;
        newPlayer.m_localSpecs = player.m_localSpecs;
        newPlayer.m_texIndex = player.m_texIndex;
        newPlayer.m_modType = player.m_modType;
        return newPlayer;
    }

public:
    void GetInput(std::shared_ptr<Input> pInput)
    {
        if (pInput->IsKeyDown(VK_RIGHT))
        {
            Move({ playerSpeed, 0.f, 0.f });
        }
        else if (pInput->IsKeyDown(VK_LEFT))
        {
            Move({ -playerSpeed, 0.f, 0.f });
        }

        if (pInput->IsKeyDown(VK_UP))
        {
            Move({ 0.f, 0.f, playerSpeed });
        }
        else if (pInput->IsKeyDown(VK_DOWN))
        {
            Move({ 0.f, 0.f, -playerSpeed });
        }
    }

private:
    void Move(XMFLOAT3 offset) override
    { m_worldSpecs.position += offset; } 

    void Rotate(XMFLOAT3 rotation) override
    { m_worldSpecs.orientation += rotation; }

private:
    //std::shared_ptr<Input> m_pInput;
   	const float playerSpeed = 0.2f;
};