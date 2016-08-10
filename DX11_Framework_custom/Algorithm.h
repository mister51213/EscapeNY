#pragma once
#include <vector>
#include "Actor.h"
#include "ISubGame.h"

using namespace std;

class Algorithm
{
public:
    Algorithm(){}

	// CODE_CHANGE: Algorithm now takes a pointer to the SubGame interface
	// Parent can be explicitly cast to pointer to a child class of ISubGame
	// like so: ChildClass *pChild = dynamic_cast<ChildClass*>(m_pGame);
	// However, if the cast fails, the returned pointer is nullptr, so it's 
	// safer to add assert( pChild != nullptr ); to test in debug mode before
	// assuming it will work, which if uses correctly, it should.
	Algorithm( ISubGame*const pGame )
    { m_pGame = pGame; }

    virtual vector<Actor> MakePattern(int numActors) = 0;
    virtual void SetData() = 0;

protected:
    ISubGame* m_pGame;
};

