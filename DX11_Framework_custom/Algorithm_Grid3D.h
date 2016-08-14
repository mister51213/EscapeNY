#pragma once
#include "Algorithm.h"

class Algorithm_Grid3D: public Algorithm
{
public:
	Algorithm_Grid3D();

	vector<Actor> MakePattern(int)override {}
	vector<Actor_NPC> MakePatternNPC( int numActors );

	void SetData()override {}
};