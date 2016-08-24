#pragma once

#include "ILight.h"
#include <vector>
#include "Utilities.h"

using namespace std;

class ILightPoint:
	public ILight
{
public:
	virtual void SetPosition( float X, float Y, float Z ) = 0;
	virtual DirectX::XMFLOAT3 GetPosition()const = 0;

    /*virtual void InitLights(unsigned int numLights=4) = 0;
    virtual unsigned int GetNumLights()const = 0;
    virtual LightColorBufferType GetColors() const = 0;
    virtual LightPositionBufferType GetPositions() const = 0;*/

};

