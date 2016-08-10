#pragma once
#include "Algorithm.h"

class Algorithm_Grid3D: public Algorithm
{
public: 
	Algorithm_Grid3D() 
	:
	Algorithm( nullptr ) 
{}
	vector<Actor> MakePattern( int numActors ) override;
	void SetData() override {}
};