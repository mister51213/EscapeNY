/////////////////////////////////////////////////////////////////
// Class name: Actor
// Function: Parent class that holds all information related 
//           to gameplay for a specific object (location, etc.).
// Example:  MovingObject->Car->Mercedes
/////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include "Utilities.h"

struct ModelSpecs_W
{
    XMFLOAT3 position, orientation, scale;
};

struct ModelSpecs_L
{
    XMFLOAT3 center, size, orientation;
};

enum eModType{CUBE, CUBE_TEXTURED, PLANE, SPHERE, POLYGON};

class Actor {
public:
    Actor(
        ModelSpecs_W worldSpecs, 
        ModelSpecs_L localSpecs = { { 0.f, 0.f, 0.f }, { 0.f,0.f,0.f }, { 1.f,1.f,1.f } }) {
    m_worldSpecs = worldSpecs;
    m_localSpecs = localSpecs;
    }

    ModelSpecs_W GetWorldSpecs()
    {
        return m_worldSpecs;
    }

    ModelSpecs_L GetLocalSpecs()
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

private:
    eModType m_modType;
    ModelSpecs_W m_worldSpecs;
    ModelSpecs_L m_localSpecs;
};