#pragma once

#include "Actor_NPC.h"
#include "Algorithm.h"

class ISubGame;

class Algorithm_Spiral3D: public Algorithm
{
public: 
	Algorithm_Spiral3D( ISubGame* pGame );

	std::vector<Actor> MakePattern( int )override { return std::vector<Actor>(); }
	std::vector<Actor_NPC> MakePatternNPC( int numActors );

	void SetData()override {}
};