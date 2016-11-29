#pragma once

#include "Actor.h"
#include "Algorithm.h"

class ISubGame;

class Algorithm_Random:
	public Algorithm
{
public:
	Algorithm_Random( ISubGame*const pGame );
	~Algorithm_Random() = default;

	std::vector<Actor> MakePattern( int numActors ) override;
	void SetData() override {}

};

