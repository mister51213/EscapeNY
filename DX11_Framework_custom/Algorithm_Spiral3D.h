#pragma once
#include "Algorithm.h"
#include "Utilities.h"


class Algorithm_Spiral3D: public Algorithm
{
public: 
	Algorithm_Spiral3D( ISubGame* pGame );

	vector<Actor> MakePattern( int )override { return vector<Actor>(); }
	vector<Actor_NPC> MakePatternNPC( int numActors );

	void SetData()override {}
};