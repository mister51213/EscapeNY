#pragma once

#include "GameView.h"
#include "Input.h"

// Sub-game includes
#include "Game_Maze.h"
#include "Game_FPS.h"
#include "Scene.h"
#include "Scene_Collision.h"

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
private:
	void updateGameObjects();
	bool render();

private:	
	// m_pGraphics and m_pDirect3D are created and passed to game 
	// without taking ownership
	Graphics *m_pGraphics;
	D3DGraphics *m_pDirect3D;

	// Input is shared between System, Game, SubGames, Camera and Actors
	std::shared_ptr<Input> m_pInput;

	// Camera is shared between Game and GameView
	std::shared_ptr<Camera> m_pCamera;
    
    GameView m_GameView;

    // This will switch to whatever game is active at the time.
    std::unique_ptr<ISubGame> m_pActiveScene;
	GameTimer m_timer; // TODO: move into GAME, make it use them when it calls m_pActiveScene.UpdateScene();
	Physics m_physics;
};

