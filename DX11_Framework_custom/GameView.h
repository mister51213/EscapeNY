////////////////////////////////////////////////////////////////////////////////
//  TODO:   Later separate this class into GameWorldObjects(for gameplay purposes)
//  TODO:   and GameRenderObjects (for rendering purposes)
///////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Camera.h"
#include "Model_Textured.h"
#include "Model.h"
#include "Graphics.h"
#include "Shader_Color.h"
#include "Texture.h"
#include "Shader_Texture.h"
#include "Actor.h"
#include "Actor_Player.h"
#include "Actor_NPC.h"
#include <string>

using namespace std;

class GameView
{
public:
	GameView();
	GameView( Graphics* pGfx, D3DGraphics* pD3D, std::shared_ptr<Camera> &pCam );
	 
	void InitializeGameObjectsSystem( const vector<Actor*>& actors );
	void UpdateView( const vector<Actor*>& actors );
	void Reset( const vector<Actor *> &pActors );
private:
	void initTexturePool();
	void initializeShader();
	void modelAllActors( const vector<Actor*>& actors );
	std::shared_ptr<Model>
		makeModel(ModelSpecs_L localSpecs, eModType Type = CUBE_TEXTURED );
	void drawModel( const Actor& actor );

private:
    Graphics* m_pGfx;
    D3DGraphics* m_pD3D;

    std::shared_ptr<Camera> m_pCam;
	Shader_Texture m_shader_Texture;
    vector<Texture> m_Textures;
};