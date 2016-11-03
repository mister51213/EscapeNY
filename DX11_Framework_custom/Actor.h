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
#include "Physics.h"

class Actor
{
public:
    Actor() = default;

    Actor(
        const ModelSpecs_W &worldSpecs,
        eTexture tex,
        const ModelSpecs_L &localSpecs,
        eModType modType = CUBE_TEXTURED);

    ///////////////
    // ACCESSORS //
    ///////////////
    eModType GetModelType()const { return m_modType; }
    eTexture GetTexIndex() const { return m_texIndex; }
    const ModelSpecs_W &GetWorldSpecs() const { return m_worldSpecs; }
    const ModelSpecs_L &GetLocalSpecs() const { return m_localSpecs; }
	PhysAttributes &GetAttributes() { return m_attributes; }

	void SetWorldSpecs(ModelSpecs_W& newSpecs)	{ m_worldSpecs = newSpecs;}
	void SetPosition(XMFLOAT3& newPos)	{ m_worldSpecs.position = newPos;}

    const XMFLOAT3 &GetPosition()const { return m_worldSpecs.position; }
    const XMFLOAT3 &GetRotation()const { return m_worldSpecs.orientation; }

    virtual void GetInput(const Input& pInput, int randI = 0.f, float randF = 0.f) {}

protected:
    eModType m_modType;
    eTexture m_texIndex;

    ModelSpecs_W m_worldSpecs;
    ModelSpecs_L m_localSpecs;

public: //TODO: MAKE PROTECTED
	PhysAttributes m_attributes;
};