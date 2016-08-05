#pragma once

#include "GameView.h"
#include "Overlay.h"
#include "Input.h"
#include <algorithm>
#include "DrawingAlgorithms.h"

class Game
{
public:
	Game(std::shared_ptr<Input> pInput);
	~Game();

	bool Initialize( Graphics *pGraphics, UINT ScreenWidth, UINT ScreenHeight, HWND WinHandle );
    void GetInput(std::shared_ptr<Input> input);

    void Game::makeActorsMASTER();

	bool Frame();
private:
	bool render();

    vector<Actor> makeActorSet(int numActors, DrawAlgorithm* algorithm);

private:	
	Graphics *m_pGraphics; // m_pGraphics and m_pDirect3D are created and passed to game without taking ownership
	D3DGraphics *m_pDirect3D;
	std::shared_ptr<Camera> m_pCamera; // Camera, Model and ColorShader are created in Game, so has ownership

    GameView m_gObjects;

    ////////////////////////////////////////////////////
    // INITIALIZATION DATA FOR m_actorsSUB1
    int m_numRows, m_numColumns, m_numZ, m_numActors;
    ////////////////////////////////////////////////////

	std::shared_ptr<Model_Textured> m_pModel1;
    std::shared_ptr<Model_Textured> m_pModel2;

    std::shared_ptr<Input> m_pInput;// Input

	Overlay m_Overlay;
    XMFLOAT3 m_camPos = { -0.0f, 16.0f, -30.0f };
    XMFLOAT3 m_camRotation = { 25.f, -5.f, 0.f }; // defined in degrees

    // sub list of actors for LIKE TYPES
    vector<Actor> m_actorsSUB1; //* vector is destroyed before the list   
    vector<Actor> m_actorsSUB2;
    vector<Actor> m_actorsSUB3;
    vector<Actor> m_actorsSUB4;

    // MASTER DRAW list for all actors    
    vector<Actor*> m_pActorsMASTER;
};

