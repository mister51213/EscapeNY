#pragma once

#include "GameView.h"
#include "Overlay.h"
#include "Input.h"
#include <algorithm>
#include "Algorithm_Grid3D.h"
#include "Algorithm_Spiral3D.h"
//#include "Actor_Player_Alt.h"
//#include "Algorithm_Maze.h"
#include "Board.h"
#include "TestBoard.h"
//#include "MazeGame.h"
#include "Game_FPS.h"

class Game
{
public:
	Game(std::shared_ptr<Input> pInput);
	~Game();

	bool Initialize(
		Graphics *pGraphics, 
		UINT ScreenWidth, 
		UINT ScreenHeight );
	
	//const TestBoard &GetBoard();
	bool Frame();
private:
	void updateGameObjects();
	void makeActorsMASTER();
    vector<Actor> makeActorSet(int numActors, Algorithm* algorithm);
	bool render();

private:	
	Graphics *m_pGraphics; // m_pGraphics and m_pDirect3D are created and passed to game without taking ownership
	D3DGraphics *m_pDirect3D;
	std::shared_ptr<Camera> m_pCamera; // Camera, Model and ColorShader are created in Game, so has ownership
    
    GameView m_GameView;
    std::shared_ptr<Input> m_pInput;// Input
    // This will switch to whatever game is active at the time.
    std::unique_ptr<ISubGame> m_pActiveGame;
};

