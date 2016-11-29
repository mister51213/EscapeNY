#pragma once

#include "Algorithm.h"
#include "Actor_NPC.h"

namespace DirectX
{
	struct XMFLOAT3;
}

class Algorithm_Grid3D: public Algorithm
{
public:
	Algorithm_Grid3D();

	std::vector<Actor> MakePattern( int )override;
	std::vector<Actor_NPC> MakePatternNPC(int numActors, DirectX::XMFLOAT3 worldOffset);

	void SetData()override {}
};