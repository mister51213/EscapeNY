#pragma once
#include <vector>
#include "Actor.h"
#include "Actor_Player.h"
#include "Actor_NPC.h"
using namespace std;

class Game;

class Algorithm
{
public:
    Algorithm(){}

    Algorithm(Game* pGame, std::shared_ptr<Input>& pInput) 
    { 
        m_pGame = pGame;
        m_pInput = pInput;
    }

    // TODO: Still make it pure virtual or just overload it instead because of difft return values?
    virtual vector<Actor> MakePattern(int numActors) = 0;
    virtual void SetData() = 0;

protected:
    Game* m_pGame;
    std::shared_ptr<Input> m_pInput;
};

