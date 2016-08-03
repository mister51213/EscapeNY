#pragma once

#include "GameObjects.h"

// TODO: Move these includes into GameObjects.h after it's done
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
	
	Graphics *m_pGraphics; // m_pGraphics and m_pDirect3D are created and passed to game without taking ownership
	D3DGraphics *m_pDirect3D;
	std::shared_ptr<Camera> m_pCamera; // Camera, Model and ColorShader are created in Game, so has ownership

    GameObjects m_gObjects;
    vector<ModelSpecs> m_modSpecList;

	std::shared_ptr<Model_Textured> m_pModel1;
    std::shared_ptr<Model_Textured> m_pModel2;

    //std::unique_ptr<ColorShader> m_pColorShader;
	std::shared_ptr<Shader_Texture> m_pShader_Texture;
	std::shared_ptr<Texture> m_pStoneTexture;
    std::shared_ptr<Input> m_pInput;// Input

	Overlay m_Overlay;
    XMFLOAT3 m_camPos = { -0.0f, 16.0f, -30.0f };
    XMFLOAT3 m_camRotation = { 25.f, -5.f, 0.f }; // defined in degrees

};

