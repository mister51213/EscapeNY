#pragma once

#include "ILight.h"

class ILightPoint:
	public ILight
{
public:
	virtual const DirectX::XMFLOAT3 &GetPosition()const = 0;

	virtual void SetPosition( float X, float Y, float Z ) = 0;
};

