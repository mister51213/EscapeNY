#pragma once

namespace DirectX
{
	struct XMFLOAT3;
}

class ILightPoint
{
public:
    virtual const DirectX::XMFLOAT3& GetPosition() const = 0;
    virtual void SetPosition(const DirectX::XMFLOAT3& position) = 0;
};

