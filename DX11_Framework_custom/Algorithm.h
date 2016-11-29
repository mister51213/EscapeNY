#pragma once

#include "Includes.h"
#include "Actor.h"

class ISubGame;

class Algorithm
{
public:
    Algorithm(){}

	// Parent can be explicitly cast to pointer to a child class of ISubGame
	// like so: ChildClass *pChild = dynamic_cast<ChildClass*>(m_pGame);
	// However, if the cast fails, the returned pointer is nullptr, so it's 
	// safer to add assert( pChild != nullptr ); to test in debug mode before
	// assuming it will work, which if used correctly, it should.
	Algorithm( ISubGame*const pGame )
    { m_pGame = pGame; }

    virtual std::vector<Actor> MakePattern(int numActors) = 0;
    virtual void SetData() = 0;

protected:
    ISubGame* m_pGame;
};

