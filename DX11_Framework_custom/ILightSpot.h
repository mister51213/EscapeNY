#pragma once

#include "ILightPoint.h"
#include "ILightDirection.h"

class ILightSpot:
	public ILightPoint
{
public:
	virtual float GetConeAngle() const = 0;
	virtual void SetConeAngle(float Angle) = 0;

   	virtual const DirectX::XMFLOAT3 &GetDirection()const = 0;
	virtual void SetDirection( const XMFLOAT3& direction ) = 0;
};

