#pragma once

class ILightDirection
{
public:
	virtual const DirectX::XMFLOAT3 &GetDirection()const = 0;
	virtual void SetDirection( float X, float Y, float Z ) = 0;
};

