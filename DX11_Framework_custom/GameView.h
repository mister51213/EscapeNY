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
	void initializeShader();
	void drawModel( const Actor& actor, MatrixBufferType &Transforms) const;

private:
    Graphics* m_pGfx;
    D3DGraphics* m_pD3D;

    std::shared_ptr<Camera> m_pCam;

    ShaderT<MatrixBufferType, LightBufferType> m_activeShader;

    vector<Texture> m_TexturePool;
    vector<std::shared_ptr<Model>> m_ModelPool;

    std::shared_ptr<Model_Textured> m_pModTEST = 0;
};