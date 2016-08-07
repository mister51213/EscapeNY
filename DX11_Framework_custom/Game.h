#pragma once

#include "GameView.h"
#include "Overlay.h"
#include "Input.h"
#include <algorithm>
#include "Algorithm_Grid3D.h"
#include "Algorithm_Spiral3D.h"
#include "Board.h"
#include "TestBoard.h"

class Game
{
public:
	Game(std::shared_ptr<Input> pInput);
	~Game();

	bool Initialize(
		Graphics *pGraphics, 
		UINT ScreenWidth, 
		UINT ScreenHeight );
	
	const TestBoard &GetBoard();
	bool Frame();
private:
	void reset();
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

	Overlay m_Overlay;
    XMFLOAT3 m_camPos = { -0.0f, 16.0f, -30.0f };

    // Camera is in top-down view for now; rotated 90 degrees on X axis
    XMFLOAT3 m_camRotation = { 90.f, 0.f, 0.f }; // defined in degrees

	Board m_board;
	TestBoard m_board;

	/////////////////
	// Test player //
	/////////////////
	Actor_Player m_player;

	////////////////////////////
	// Maze game only members //
	////////////////////////////
	bool m_endReached = false;

    // sub list of actors for LIKE TYPES
    vector<Actor_NPC> m_actorsSUB1; //* vector is destroyed before the list   
    vector<Actor_NPC> m_actorsSUB2;
    vector<Actor_NPC> m_actorsSUB3;
    vector<Actor_NPC> m_actorsSUB4;

    // MASTER DRAW list for all actors    
    vector<Actor*> m_pActorsMASTER;
};

