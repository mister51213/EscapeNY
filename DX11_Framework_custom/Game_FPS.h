#pragma once
#include "ISubGame.h";

class Game_FPS:public ISubGame
{
    virtual void Initialize(
        Graphics *pGraphics,
        class Game *const pGame,
        Camera *const pCamera) override;

	// Use UpdateFrame to update actors, camera and anything else that 
	// needs updating.
    virtual void UpdateFrame(const Input &InputRef) override;

	// Use RenderFrame to render the list of actors or other game objects
	virtual void RenderFrame( const GameView &GameViewRef ) override;
};