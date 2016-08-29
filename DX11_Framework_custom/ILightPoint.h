#pragma once

#include "Utilities.h"

class ILightPoint
{
public:
    virtual const XMFLOAT3& GetPosition() const = 0;
    virtual void SetPosition(const XMFLOAT3& position) = 0;
};

