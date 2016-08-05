#pragma once
#include <vector>
#include "Actor.h"
using namespace std;

class Game;

class Algorithm
{
public:
    Algorithm(){}

    Algorithm(Game* pGame) 
    { m_pGame = pGame; }

    virtual vector<Actor> MakePattern(int numActors) = 0;
    virtual void SetData() = 0;

protected:
    Game* m_pGame;
};

