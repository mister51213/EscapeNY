#pragma once

#include "Light_Base.h"
#include "ILightDirection.h"

class Light_Diffuse :
	public Light_Base, public ILightDirection
{
public:
	Light_Diffuse()
		:
		Light_Base( eLightType::INFINITE_LIGHT )
	{
	}

	~Light_Diffuse() = default;

	const DirectX::XMFLOAT3 &GetDirection()const override;

	void SetDirection( float X, float Y, float Z )override;

};
