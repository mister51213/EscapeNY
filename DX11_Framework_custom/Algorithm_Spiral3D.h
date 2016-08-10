#pragma once

#include "Utilities.h"
#include "Algorithm.h"
#include "ISubGame.h"

class Algorithm_Spiral3D: public Algorithm
{
public:
	Algorithm_Spiral3D( ISubGame* pGame );
	vector<Actor> MakePattern( int numActors ) override;
	void SetData() override {}
};