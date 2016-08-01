#pragma once

//#include "GameObjects.h"

// TODO: Move these includes into GameObjects.h
#include "Camera.h"
#include "Model.h"
#include "Graphics.h"
#include "Shader_Color.h"
#include "Texture.h"
#include "Shader_Texture.h"
#include "Overlay.h"
#include "Input.h"

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
private:
	// m_pGraphics and m_pDirect3D are created and passed to game without taking ownership
	Graphics *m_pGraphics;
	D3DGraphics *m_pDirect3D;

	// Camera, Model and ColorShader are created in Game, so has ownership
	std::unique_ptr<Camera> m_pCamera;

    // TODO: Individual objects shouldn't be held in a member pointer;
    // should be passed as parameters to draw individual objects to a single rendering pipeline
    //GameObjects m_objects;

	std::unique_ptr<Model_Textured> m_pModel;

    // TODO: Add new model here as experiment
    std::unique_ptr<Model_Textured> m_pModel2;

    //std::unique_ptr<ColorShader> m_pColorShader;
	std::unique_ptr<Shader_Texture> m_pShader_Texture;
	std::unique_ptr<Texture> m_pStoneTexture;
	Overlay m_Overlay;

// later add this to GameObjects

    XMFLOAT3 m_modelOffset = { 0,0,0 };
    XMFLOAT3 m_camPos = { -0.0f, 8.0f, -16.0f };
    XMFLOAT3 m_camRotation = { 25.f, -5.f, 0.f };

// Input
    std::shared_ptr<Input> m_pInput;
};

