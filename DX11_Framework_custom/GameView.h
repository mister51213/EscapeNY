////////////////////////////////////////////////////////////////////////////////
//  TODO:   Later separate this class into GameWorldObjects(for gameplay purposes)
//  TODO:   and GameRenderObjects (for rendering purposes)
///////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ResourceManager.h"
#include "Camera.h"
#include "Model_Textured.h"
#include "Model.h"
#include "Graphics.h"
#include "Shader_Color.h"
#include "Texture.h"
#include "Shader_Texture.h"
#include "Actor.h"
#include <string>

using namespace std;

class GameView
{
public:
	GameView();
	
	 
	// CODE_CHANGE: Initialize function no longer needs vector of Actor pointers
	void Initialize( 
		ResourceManager *const pResource, 
		Graphics* pGfx,
		Camera* pCam );
	void UpdateView( const vector<Actor*>& actors )const;

	// CODE_CHANGE: Renamed function to OnReset to be more clear of intent and
	// made const, none of the functions called from here change the 
	// state of GameView members.
	void OnReset( const vector<Actor *> &pActors )const;
private:
	void initTexturePool();
	void initializeShader();
	// CODE_CHANGE: Made const
	void modelAllActors( const vector<Actor*>& actors )const;
	// CODE_CHANGE: Made const
	std::shared_ptr<Model>
		makeModel(ModelSpecs_L localSpecs, eModType Type = CUBE_TEXTURED )const;
	void drawModel( const Actor& actor )const;

private:
    Graphics* m_pGfx;
    D3DGraphics* m_pD3D;

	Camera *m_pCam;
	Shader_Texture m_shader_Texture;
	ResourceManager *m_pResource;
    //vector<Texture> m_Textures;
};