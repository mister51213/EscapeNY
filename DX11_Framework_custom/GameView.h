////////////////////////////////////////////////////////////////////////////////
//  TODO:   Later separate this class into GameWorldObjects(for gameplay purposes)
//  TODO:   and GameRenderObjects (for rendering purposes)
///////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Camera.h"
#include "Model_Textured.h"
#include "Model.h"
#include "Graphics.h"
#include "Texture.h"
#include "Actor.h"
#include "Actor_Player.h"
#include "Actor_NPC.h"
#include <string>
#include "ILight.h"
#include "ShaderTEMPLATE.h"
#include "Shader_NMap.h"

using namespace std;

class GameView
{
public:
	GameView();
	GameView( Graphics* pGfx, D3DGraphics* pD3D, std::shared_ptr<Camera> &pCam );
	 
    void Initialize();
	void UpdateView( const vector<Actor*>& actors, const vector<LightBufferType>& lightSet) const;
    void initModelPool();
private:
	void initTexturePool();
    void initNormalMapPool();
	void initializeShader();
	void drawModel( const Actor& actor, MatrixBufferType &Transforms) const;

private:
    Graphics* m_pGfx;
    D3DGraphics* m_pD3D;

    std::shared_ptr<Camera> m_pCam;

    ShaderT<
		VertexBufferTypeAllInOne, 
		MatrixBufferType, 
		LightBufferType> m_activeShader;
	ShaderT<
		VertexBufferTypeAllInOneNMap, 
		MatrixBufferType, 
		LightBufferTypeNMap> m_NMapShader;
	Shader_NMap m_shader_nMap;
	
    vector<Texture> m_TexturePool;
    vector<Texture> m_NormalPool;
    vector<std::shared_ptr<Model>> m_ModelPool;

    std::shared_ptr<Model_Textured> m_pModTEST = 0;
};