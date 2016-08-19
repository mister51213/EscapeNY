#pragma once

#include "ISubGame.h"
#include "AnimatedLight_Square.h"
#include "AnimatedLight_Pendulum.h"

class Game_LightTests:
	public ISubGame
{
public:
	 Game_LightTests() = default;
	~Game_LightTests() = default;

	// Use Initialize to initialize anything that needs initializing.
	void Initialize(
		Graphics *pGraphics,
		class Game *const pGame,
		Camera *const pCamera )override final;

	// Use UpdateFrame to update actors, camera and anything else that 
	// needs updating.
	void UpdateScene( const Input &InputRef, Camera *const pCamera ) override final;

	// Use RenderFrame to render the list of actors or other game objects
	void RenderFrame( const GameView &GameViewRef ) override final;

private:
	Graphics *m_pGraphics;
	Game *m_pGame;
	Camera *m_pCamera;
	LightSpotBase m_spotLight;
	AnimatedLight_Square m_animatedLight;
	AnimatedLight_Pendulum m_animatedPendulum;
	Actor_NPC m_plane;
	vector<Actor*> m_pActorDrawList;

};

