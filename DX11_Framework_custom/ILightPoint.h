#pragma once

#include "ILight.h"
#include <vector>
#include "Utilities.h"

using namespace std;

class ILightPoint:
	public ILight
{
public:
    virtual const XMFLOAT3& GetPosition() const = 0;
    virtual void SetPosition(const XMFLOAT3& position) = 0;
};

