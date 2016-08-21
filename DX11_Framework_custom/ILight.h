#pragma once

#include "Includes.h"
#include "Utilities.h"

class ILight
{
public:
    virtual const DirectX::XMFLOAT4 &GetAmbient()const = 0;
	virtual void SetAmbient( float R, float G, float B ) = 0;

	virtual const DirectX::XMFLOAT4 &GetColor()const = 0;
	virtual float GetIntensity()const = 0;

	virtual void SetColor( float R, float G, float B ) = 0;
	virtual void SetIntensity( float Value ) = 0;
};

