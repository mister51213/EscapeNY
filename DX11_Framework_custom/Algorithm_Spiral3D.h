#pragma once
#include "Algorithm.h"
#include "Utilities.h"


class Algorithm_Spiral3D: public Algorithm
{
public: 
	Algorithm_Spiral3D( ISubGame* pGame );

	// I'm surprised this class is instiatable, since you aren't
	// overriding the base pure virtual MakePattern function
	vector<Actor_NPC> MakePatternNPC( int numActors );

	void SetData() {}
};