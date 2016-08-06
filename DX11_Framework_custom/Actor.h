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
		ModelSpecs_L localSpecs = { { 0.f, 0.f, 0.f },{ 5.f, 5.f, 5.f },{ 0.f, 0.f, 0.f } } )
	{
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

	void Move( XMFLOAT3 offset )
	{
		m_worldSpecs.position += offset;
	}

	void Rotate( XMFLOAT3 rotation )
	{
		m_worldSpecs.orientation += rotation;
	}

private:
	eModType m_modType;
	//string m_name;
	ModelSpecs_W m_worldSpecs;
	ModelSpecs_L m_localSpecs;
};