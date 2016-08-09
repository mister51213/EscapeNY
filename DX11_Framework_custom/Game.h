#pragma once

#include "GameView.h"
#include "Overlay.h"
#include "Input.h"
#include <algorithm>
#include "Actor_Player_Alt.h"
#include "Algorithm_Maze.h"
#include "Board.h"
#include "TestBoard.h"
#include "MazeGame.h"

class Game
{
public:
	Game(std::shared_ptr<Input> pInput);
	~Game();

	bool Initialize(
		Graphics *pGraphics, 
		UINT ScreenWidth, 
		UINT ScreenHeight );
	
	bool Frame();
	const GameView &GetGameView()const;
private:
	void updateGameObjects();
	bool render();

private:	
	Graphics *m_pGraphics; // m_pGraphics and m_pDirect3D are created and passed to game without taking ownership
	D3DGraphics *m_pDirect3D;

	Camera m_camera;
    GameView m_GameView;
    std::shared_ptr<Input> m_pInput;
	std::unique_ptr<ISubGame> m_pActiveGame; 
};

