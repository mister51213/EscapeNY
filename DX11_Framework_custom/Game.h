#pragma once

#include "Camera.h"
#include "Model.h"
#include "Graphics.h"
#include "Shader_Color.h"
#include "Texture.h"
#include "Shader_Texture.h"
#include "Overlay.h"

class Game
{
public:
	Game();
	~Game();

	bool Initialize( Graphics *pGraphics, UINT ScreenWidth, UINT ScreenHeight, HWND WinHandle );
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
	std::unique_ptr<Model_Textured> m_pModel;

    // TODO: Add new model here as experiment
    std::unique_ptr<Model_Textured> m_pModel2;

    //std::unique_ptr<ColorShader> m_pColorShader;
	std::unique_ptr<Shader_Texture> m_pShader_Texture;
	std::unique_ptr<Texture> m_pStoneTexture;
	Overlay m_Overlay;};

