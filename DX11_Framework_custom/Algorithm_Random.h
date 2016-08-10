#pragma once
#include "Algorithm.h"
class Algorithm_Random:
	public Algorithm
{
public:
	Algorithm_Random( ISubGame*const pGame );
	~Algorithm_Random();

	vector<Actor> MakePattern( int numActors ) override;
	void SetData() override {}

};

