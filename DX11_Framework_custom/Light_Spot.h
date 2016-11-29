#pragma once

#include "Light_Base.h"
#include "ILightSpot.h"

class Light_Spot :
	public Light_Base, 
	public ILightSpot
{
public:

	Light_Spot()
		:
		Light_Base( eLightType::SPOT_LIGHT )
	{}

	~Light_Spot() = default;

	const DirectX::XMFLOAT3 & Light_Spot::GetDirection() const override;
	void Light_Spot::SetDirection( float X, float Y, float Z ) override;
	const DirectX::XMFLOAT3 & Light_Spot::GetPosition() const override;
	void Light_Spot::SetPosition( const DirectX::XMFLOAT3& position ) override;
	const DirectX::XMFLOAT4 & Light_Spot::GetColor() const override;
	void SetConeAngle( float OuterAngle ) override;
	float GetConeAngle()const override;
};

