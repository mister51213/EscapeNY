#pragma once

#include "Light_Base.h"
#include "ILightPoint.h"

namespace DirectX{ struct XMFLOAT3; }

class Light_Point :
	public Light_Base,
	public ILightPoint
{
public:

	Light_Point()
		:
		Light_Base( eLightType::POINT_LIGHT )
	{}

	~Light_Point() = default;

	const DirectX::XMFLOAT3& GetPosition() const override;

	void SetPosition( const DirectX::XMFLOAT3& position ) override;


private:
	LightBufferType m_lightBuffer;
};

