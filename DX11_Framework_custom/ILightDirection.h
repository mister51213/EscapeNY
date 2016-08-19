#pragma once
#include "ILight.h"
class ILightDirection:
	public ILight
{
public:
	virtual const DirectX::XMFLOAT3 &GetDirection()const = 0;
	virtual void SetDirection( float X, float Y, float Z ) = 0;
};

