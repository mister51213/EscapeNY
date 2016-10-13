#pragma once

#include "ILightDirection.h"
#include "ILightPoint.h"

class ILightSpot :
	public ILightPoint,
	public ILightDirection
{
public:
	virtual float GetConeAngle() const = 0;
	virtual void SetConeAngle(float Angle) = 0;
};

