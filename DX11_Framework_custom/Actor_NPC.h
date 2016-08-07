#pragma once
#include "Actor.h"

class Actor_NPC : public Actor
{
public:
    Actor_NPC() {}

    Actor_NPC(
        std::shared_ptr<Input> pInput,
        ModelSpecs_W worldSpecs,
        eTexture tex,
        ModelSpecs_L localSpecs,
        eModType modType = CUBE_TEXTURED) /*:
        Actor(worldSpecs, tex, localSpecs, modType = CUBE_TEXTURED)*/ // base constructor
    { 
        m_pInput = pInput;
        m_worldSpecs = worldSpecs;
        m_localSpecs = localSpecs;
        m_texIndex = tex;
        m_modType = modType;
    }

public:
    void Move(XMFLOAT3 offset) override
    { m_worldSpecs.position += offset; } 

    void Rotate(XMFLOAT3 rotation) override
    { m_worldSpecs.orientation += rotation; }

private: // TODO: only in Player class
    std::shared_ptr<Input> m_pInput;
};
