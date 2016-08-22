////////////////////////////////////////////////////////////////////////////////
//  TODO:   Later separate this class into GameWorldObjects(for gameplay purposes)
//  TODO:   and GameRenderObjects (for rendering purposes)
///////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Camera.h"
#include "Model_Textured.h"
#include "Model_Colored.h"
#include "Model.h"
#include "Graphics.h"
#include "Shader_Color.h"
#include "Texture.h"
#include "Shader_Texture.h"
#include "Shader_Lighting.h"
#include "Shader_PtLight.h"
#include "Actor.h"
#include "Actor_Player.h"
#include "Actor_NPC.h"
#include <string>
#include "ILight.h"

using namespace std;

class GameView
{
public:
	GameView();
	GameView( Graphics* pGfx, D3DGraphics* pD3D, std::shared_ptr<Camera> &pCam );
	 
    void Initialize();
	void UpdateView( const vector<Actor*>& actors, ILight* pLight) const;
    void initModelPool();
private:
	void initTexturePool();
	void initializeShader();
	void drawModel( const Actor& actor, ILight* pLight) const;

private:
    Graphics* m_pGfx;
    D3DGraphics* m_pD3D;

    std::shared_ptr<Camera> m_pCam;
	//Shader_Texture m_shader_Texture;
    Shader_Color m_shader_Color;
    Shader_Lighting m_shader_Lighting;
    Shader_PtLight m_shader_LightingPT;

    vector<Texture> m_TexturePool;
    vector<std::shared_ptr<Model>> m_ModelPool;

    std::shared_ptr<Model_Textured> m_pModTEST = 0;
};