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
	// CODE_CHANGE: Implement reset() if needed in ISubGame children.
	//void reset();
	void updateGameObjects();
	void makeActorsMASTER();
	void getInput( std::shared_ptr<Input> input );
    vector<Actor> makeActorSet(int numActors, Algorithm* algorithm);	
	bool render();

private:	
	Graphics *m_pGraphics; // m_pGraphics and m_pDirect3D are created and passed to game without taking ownership
	D3DGraphics *m_pDirect3D;
	std::shared_ptr<Camera> m_pCamera; // Camera, Model and ColorShader are created in Game, so has ownership

    GameView m_GameView;

    std::shared_ptr<Input> m_pInput;// Input

	std::unique_ptr<ISubGame> m_pActiveGame;
    //XMFLOAT3 m_camPos = { -0.0f, 16.0f, -30.0f };

    // Camera is in top-down view for now; rotated 90 degrees on X axis
    //XMFLOAT3 m_camRotation = { 90.f, 0.f, 0.f }; // defined in degrees

	//Actor m_player;


    // sub list of actors for LIKE TYPES
    //vector<Actor> m_actorsSUB1; //* vector is destroyed before the list   
    //vector<Actor> m_actorsSUB2;
    //vector<Actor> m_actorsSUB3;
    //vector<Actor> m_actorsSUB4;

    // MASTER DRAW list for all actors    
    //vector<Actor*> m_pActorsMASTER;
};

