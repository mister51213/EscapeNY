#pragma once

#include "ISubGame.h"
#include "Overlay.h"
#include "TestBoard.h"
#include "Algorithm_Maze.h"

class MazeGame:
	public ISubGame
{
public:
	MazeGame();
	~MazeGame();

	// Use Initialize to initialize anything that needs initializing.
	void Initialize( Graphics *pGraphics, Game *const pGame ) override;

	// Use UpdateFrame to update actors, camera and anything else that 
	// needs updating.
	void UpdateFrame( const Input &InputRef, Camera *const pCamera ) override;

	// Use RenderFrame to render the list of actors or other game objects
	void RenderFrame( const GameView &GameViewRef ) override;

	const TestBoard &GetBoard();

private: // Private methods
	void reset();

private: // Private members
	// Game and Graphics pointers are not owned by MazeGame, do not
	// delete
	Game *m_pGame;
	Graphics *m_pGraphics;

	Overlay m_Overlay;
	TestBoard m_board;

	Actor m_player;
	std::vector<Actor*> m_pActorDrawList;

	bool m_endReached = false;
};

