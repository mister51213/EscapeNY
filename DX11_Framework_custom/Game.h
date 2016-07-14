#pragma once

#include "Camera.h"
#include "Model.h"
#include "Graphics.h"
#include "ColorShader.h"
#include "Texture.h"

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
	std::unique_ptr<Model> m_pModel;
	std::unique_ptr<ColorShader> m_pColorShader;
	std::unique_ptr<Texture> m_pStoneTexture;
};

