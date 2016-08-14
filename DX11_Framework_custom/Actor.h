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
	Actor() = default;

	Actor(
		const ModelSpecs_W &worldSpecs,
		eTexture tex,
		const ModelSpecs_L &localSpecs,
		eModType modType = CUBE_TEXTURED );

	///////////////
	// ACCESSORS //
	///////////////
	eModType GetModelType()const { return m_modType; }
	eTexture GetTexIndex() const { return m_texIndex; }
	const ModelSpecs_W &GetWorldSpecs() const { return m_worldSpecs; }
	const ModelSpecs_L &GetLocalSpecs() const { return m_localSpecs; }

	// TODO: move these into a child class, they are redundant for most
	// If they are redundant, wouldn't you want them in parent, so you 
	// don't have to keep remaking the same function?
	const XMFLOAT3 &GetPosition()const { return m_worldSpecs.position; }
	const XMFLOAT3 &GetRotation()const { return m_worldSpecs.orientation; }

	////////////////
	// MOVE LOGIC //
	////////////////
	virtual void GetInput( const Input& pInput, int randI, float randF ) = 0;

	// |	: private inheritance?  Probably should be protected,
	// V   	  or they should be removed from parent all together	  
private:
	virtual void Move( const XMFLOAT3 &offset ) = 0;
	virtual void Rotate( const XMFLOAT3 &rotation ) = 0;

protected:
	eModType m_modType;
	eTexture m_texIndex;

	ModelSpecs_W m_worldSpecs;
	ModelSpecs_L m_localSpecs;
};