#pragma once

#include "Includes.h"
#include "Utilities.h"
#include "ILightSpot.h"

class AnimatedLight_Pendulum
{
public:
	AnimatedLight_Pendulum()  = default;
	~AnimatedLight_Pendulum() = default;

	void MakePattern( const DirectX::XMFLOAT3 &PivotPosition, float Length );
	void Update( ILightSpot *pLight, float DeltaTime );
private:
	float m_speed = 0.f;
	float m_length = 0.f;
	DirectX::XMFLOAT3 m_pivotPosition;
	DirectX::XMFLOAT3 m_velocity;
};

