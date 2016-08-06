/////////////////////////////////////////////////////////////////
// Class name: Actor
// Function: Parent class that holds all information related 
//           to gameplay for a specific object (location, etc.).
// Example:  MovingObject->Car->Mercedes
/////////////////////////////////////////////////////////////////
#pragma once

#include "Includes.h"
#include "Utilities.h"
#include "Model.h"

class Actor
{
public:
    Actor() {}

    Actor(
        ModelSpecs_W worldSpecs,
        eTexture tex = AsphaltOld,
        ModelSpecs_L localSpecs = { { 0.f, 0.f, 0.f },{ 5.f, 5.f, 5.f },{ 0.f, 0.f, 0.f } },
        eModType modType = CUBE_TEXTURED)
    {
        m_worldSpecs = worldSpecs;
        m_localSpecs = localSpecs;
        m_texIndex = tex;
        m_modType = modType;

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

	void SetModel(const std::shared_ptr<Model>& pMod)
    {
        m_pModel = pMod;
    }


protected:
    eModType m_modType;
    eTexture m_texIndex;

	ModelSpecs_W m_worldSpecs;
    ModelSpecs_L m_localSpecs;

    std::shared_ptr<Model> m_pModel;
};