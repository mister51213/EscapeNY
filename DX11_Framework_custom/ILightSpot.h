#pragma once

#include "ILightPoint.h"
#include "ILightDirection.h"

class ILightSpot:
	public ILightPoint, public ILightDirection
{
public:

};

