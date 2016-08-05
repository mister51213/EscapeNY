#pragma once

#include "GameWorld.h"
#include "Overlay.h"
#include "Input.h"
#include <algorithm>
#include "Board.h"

class Game
{
public:
	Game(std::shared_ptr<Input> pInput);
	~Game();

	bool Initialize( Graphics *pGraphics, UINT ScreenWidth, UINT ScreenHeight, HWND WinHandle );
    void GetInput(std::shared_ptr<Input> input);

	bool Frame();
private:
	bool render();
	std::shared_ptr<Model> makeColoredModel(
		const XMFLOAT4 &Color, Actor *const pActor, eModType Type
	);
    void makeAllActors(int numActors);

private:
	
	Graphics *m_pGraphics; // m_pGraphics and m_pDirect3D are created and passed to game without taking ownership
	D3DGraphics *m_pDirect3D;
	std::shared_ptr<Camera> m_pCamera; // Camera, Model and ColorShader are created in Game, so has ownership

	Board m_board;
    GameWorld m_gObjects;
    int m_numRows, m_numColumns, m_numZ, m_numActors;

	std::shared_ptr<Model_Textured> m_pModel1;
    std::shared_ptr<Model_Textured> m_pModel2;

    std::shared_ptr<Input> m_pInput;// Input

	Overlay m_Overlay;
    XMFLOAT3 m_camPos = { -0.0f, 16.0f, -30.0f };
    XMFLOAT3 m_camRotation = { 25.f, -5.f, 0.f }; // defined in degrees

	// Not what I mean, should just be a vector<Actor *> because you
	// are creating a concrete list of child actors, then storing their
	// addresses.  If you use shared_ptr on stack allocated objects,
	// the program will crash.
    //vector<std::shared_ptr<Actor>> m_actors;
	vector<Actor *> m_pActors;
	vector<shared_ptr<Model>> m_pModels;

};

