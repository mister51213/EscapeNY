#pragma once

#include "ILightPoint.h"
#include "ILightDirection.h"

class ILightSpot:
	public ILightPoint, public ILightDirection
{
public:
	virtual void SetInnerConeAngle(float InnerAngle) = 0;
	virtual void SetOuterConeAngle(float OuterAndle) = 0;

	virtual float GetInnerConeAngle()const = 0;
	virtual float GetOuterConeAngle()const = 0;
};

