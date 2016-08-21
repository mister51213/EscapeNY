#pragma once

#include "ILight.h"
#include <vector>
#include "Utilities.h"

using namespace std;

class ILightPoint:
	public ILight
{
public:
    virtual void InitLights(unsigned int numLights=4) = 0;
    //virtual vector<PtLightBufferType> GetLightList()const = 0;

    virtual unsigned int GetNumLights()const = 0;

    virtual LightColorBufferType GetColors() const = 0;
    virtual LightPositionBufferType GetPositions() const = 0;

};

