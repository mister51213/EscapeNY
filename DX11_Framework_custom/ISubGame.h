#pragma once

#include "Camera.h"
#include "GameView.h"
#include "Input.h"
// Will need to include the actual child headers, not just the
// parent Actor.h file to access the children
#include "Actor.h"

class ISubGame
{
public:
	// Use Initialize to initialize anything that needs initializing.
	virtual void Initialize( Graphics *pGraphics, Game *const pGame) = 0;

	// Use UpdateFrame to update actors, camera and anything else that 
	// needs updating.
	virtual void UpdateFrame( const Input &InputRef, Camera *const pCamera ) = 0;

	// Use RenderFrame to render the list of actors or other game objects
	virtual void RenderFrame( const GameView &GameViewRef ) = 0;

};

