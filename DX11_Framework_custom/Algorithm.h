#pragma once
#include <vector>
#include "Actor.h"
#include "Actor_Player.h"
#include "Actor_NPC.h"
#include "ISubGame.h"
using namespace std;

class Game;

class Algorithm
{
public:
    Algorithm(){}

    Algorithm(ISubGame* pGame/*, std::shared_ptr<Input>& pInput*/) 
    { 
        m_pGame = pGame;
        //m_pInput = pInput;
    }

    // TODO: Still make it pure virtual or just overload it instead because of difft return values?
    virtual vector<Actor> MakePattern(int numActors) { return vector<Actor>(); }
    virtual void SetData() {}

protected:
    Game* m_pGame;
    std::shared_ptr<Input> m_pInput;
};

