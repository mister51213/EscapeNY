#pragma once

#include "Camera.h"
#include "GameView.h"
#include "Input.h"

class ISubGame
{
public:
	// Use Initialize to initialize anything that needs initializing.
	virtual void Initialize(
		Graphics *pGraphics, 
		class Game *const pGame,
		Camera *const pCamera) = 0;

	// Use this to update actors, camera and anything else that needs updating.
	virtual void UpdateScene( const Input &InputRef, Camera *const pCamera) = 0;

	// Use RenderFrame to render the list of actors or other game objects
	virtual void RenderFrame( const GameView &GameViewRef ) = 0;

};
