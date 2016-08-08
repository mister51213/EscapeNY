#pragma once

#include "ISubGame.h"
#include "Overlay.h"
#include "TestBoard.h"
#include "Algorithm_Maze.h"
#include "Actor_Player_Alt.h"

class MazeGame:
	public ISubGame
{
public:
	MazeGame();
	~MazeGame();

	// Use Initialize to initialize anything that needs initializing.
	void Initialize( 
		Graphics *pGraphics, 
		class Game *const pGame,
		Camera *const pCamera ) override;

	// Use UpdateFrame to update actors, camera and anything else that 
	// needs updating.
	void UpdateFrame( const Input &InputRef ) override;

	// Use RenderFrame to render the list of actors or other game objects
	void RenderFrame( const GameView &GameViewRef ) override;

	const TestBoard &GetBoard();

private: // Private methods
	void reset();

private: // Private members
	// Game, Graphics and Camera pointers are not owned by MazeGame, do not
	// delete
	Game *m_pGame;
	Graphics *m_pGraphics;
	Camera *m_pCamera;

	Overlay m_Overlay;
	TestBoard m_board;

	Actor_Player_Alt m_player;
	std::vector<Actor*> m_pActorDrawList;

	bool m_endReached = false;
};

