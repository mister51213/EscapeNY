#pragma once

#include "Camera.h"
#include "GameView.h"
#include "Input.h"

class ISubGame
{
protected:
	// Making all functions in interface protected since there is no
	// implementation in the interface.  The implementations can be
	// public in the child classes.

	// Use Initialize to initialize anything that needs initializing.
	virtual void Initialize() = 0;

	// Use UpdateFrame to update actors, camera and anything else that 
	// needs updating.
	virtual void UpdateFrame( const Input &InputRef, const Camera &CameraRef ) = 0;

	// Use RenderFrame to render the list of actors or other game objects
	virtual void RenderFrame( const GameView &GameViewRef ) = 0;

};

