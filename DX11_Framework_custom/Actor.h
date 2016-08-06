/////////////////////////////////////////////////////////////////
// Class name: Actor
// Function: Parent class that holds all information related 
//           to gameplay for a specific object (location, etc.).
// Example:  MovingObject->Car->Mercedes
/////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include "Utilities.h"

class Actor
{
public:
    Actor() {}

    Actor(
        ModelSpecs_W worldSpecs,
        eTexture tex = SharkSkin,
        ModelSpecs_L localSpecs = { { 0.f, 0.f, 0.f }, { 0.f,0.f,0.f }, { 1.f,1.f,1.f } },
        eModType modType = CUBE_TEXTURED)
    {
        m_worldSpecs = worldSpecs;
        m_texIndex = tex;
    }

    ModelSpecs_W GetWorldSpecs() const
    {
        return m_worldSpecs;
    }

    ModelSpecs_L GetLocalSpecs() const
    {
        return m_localSpecs;
    }

    void Move(XMFLOAT3 offset)
    {
        m_worldSpecs.position += offset;
    }

    void Rotate(XMFLOAT3 rotation)
    {
        m_worldSpecs.orientation += rotation;
    }

    //std::shared_ptr<Texture> GetTexture() const
    //{
    //    return m_pTexture;
    //}

    eTexture GetTexIndex() const
    {
        return m_texIndex;
    }

    std::shared_ptr<Model> GetModel() const
    {
        return m_pModel;
    }
    void SetModel(std::shared_ptr<Model> pMod)
    {
        m_pModel = pMod;
    }


private:
    eModType m_modType;
    eTexture m_texIndex;

    //string m_name;
    ModelSpecs_W m_worldSpecs;
    ModelSpecs_L m_localSpecs;

    //std::shared_ptr<Texture> m_pTexture;
    std::shared_ptr<Model> m_pModel;
};