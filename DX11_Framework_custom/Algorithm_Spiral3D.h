#pragma once
#include "Algorithm.h"


class Algorithm_Spiral3D: public Algorithm
{
public: 
	Algorithm_Spiral3D( ISubGame* pGame );

	std::vector<Actor> MakePattern( int )override { return std::vector<Actor>(); }
	std::vector<Actor_NPC> MakePatternNPC( int numActors );

	void SetData()override {}
};