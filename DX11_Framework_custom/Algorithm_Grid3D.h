#pragma once
#include "Algorithm.h"

class Algorithm_Grid3D: public Algorithm
{
public:
	Algorithm_Grid3D();

	vector<Actor> MakePattern( int )override { return vector<Actor>(); }
	vector<Actor_NPC> MakePatternNPC(int numActors, XMFLOAT3 worldOffset);

	void SetData()override {}
};