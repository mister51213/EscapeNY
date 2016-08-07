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
#include "Input.h"

class Actor
{
public:
    Actor() {}

    Actor(
        ModelSpecs_W worldSpecs,
        eTexture tex,
        ModelSpecs_L localSpecs,
        eModType modType = CUBE_TEXTURED)
    {
        m_worldSpecs = worldSpecs;
        m_localSpecs = localSpecs;
        m_texIndex = tex;
        m_modType = modType;
    }

    ///////////////
    // ACCESSORS //
    ///////////////
    ModelSpecs_W GetWorldSpecs() const
    { return m_worldSpecs; }

    ModelSpecs_L GetLocalSpecs() const
    { return m_localSpecs; }
    
    eTexture GetTexIndex() const
    { return m_texIndex; }

    std::shared_ptr<Model> GetModel() const
    { return m_pModel; }

	void SetModel(const std::shared_ptr<Model>& pMod)
    { m_pModel = pMod; }

    // TODO: move these into a child class, they are redundant for most
  	const XMFLOAT3 &GetPosition()const
	{ return m_worldSpecs.position; }

	const XMFLOAT3 &GetRotation()const
	{ return m_worldSpecs.orientation; }

    ////////////////
    // MOVE LOGIC //
    ////////////////
    void GetInput(std::shared_ptr<Input> pInput)
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


    // TODO: take this OUT of parent (Some classes wont need to move, like walls)
    virtual void Move(XMFLOAT3 offset)
            { m_worldSpecs.position += offset; } 
    virtual void Rotate(XMFLOAT3 rotation)
            { m_worldSpecs.orientation += rotation; }

protected:
    eModType m_modType;
    eTexture m_texIndex;

	ModelSpecs_W m_worldSpecs;
    ModelSpecs_L m_localSpecs;

    std::shared_ptr<Model> m_pModel;  	
};