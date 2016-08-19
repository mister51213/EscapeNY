#pragma once

#include "Includes.h"
#include "ILightPoint.h"

class AnimatedLight_Square
{
public:
	AnimatedLight_Square();
	~AnimatedLight_Square() = default;

	// StartPosition is where the pattern will start
	// Count is the number of points to produce
	// Scale is how far apart the furthest points will be from each other
	void MakePattern( 
		const DirectX::XMFLOAT3 &StartPosition, UINT Count, 
		float Scale, float Speed );
	// The class itself handles moving the light, so just call update
	// passing the ILightPoint interface pointer
	void Update( ILightPoint *const pLight, float DetlaTime );

private:
	std::vector<DirectX::XMFLOAT3> m_points;
	int m_curIndex;  // current index of point to visit
	float m_speed;
};

