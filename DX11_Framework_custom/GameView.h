////////////////////////////////////////////////////////////////////////////////
//  TODO:   Later separate this class into GameWorldObjects(for gameplay purposes)
//  TODO:   and GameRenderObjects (for rendering purposes)
///////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Model_Textured.h"
#include "Actor.h"
#include "Shader_NMap.h"
#include "Texture.h"

class Camera;
class Graphics;
class D3DGraphics;

/*
TODO: Implement; sub groups of textures and models for different chapters
TODO: Tie these in with CHAPTER struct.
VisualTheme* m_activeTheme;
VisualTheme* m_theme1;
VisualTheme* m_theme1;
VisualTheme* m_theme1;
VisualTheme* m_theme1;


// Load each visual theme as needed
LoadCurrentTheme(eTheme indexOfCurrentTheme);

// one model pool which is reskinned per level
ModelPool m_modelPool;;
// TexturePool is now broken up into many different themes
*/
class GameView
{
public:
	GameView();
	GameView( Graphics* pGfx, D3DGraphics* pD3D, Camera *pCam );
	 
    void Initialize();
	void UpdateView( const std::vector<Actor*>& actors, const SceneBufferType& SceneLights ) const;
    void initModelPool();
private:
	void initTexturePool();
    void initNormalMapPool();
	void initializeShader();
	void drawModel( const Actor& actor, MatrixBufferType &Transforms) const;

private:
    Graphics* m_pGfx;
    D3DGraphics* m_pD3D;
	Camera *m_pCam;

	Shader_NMap m_shader_nMap;
	
    std::vector<Texture> m_TexturePool;
    std::vector<Texture> m_NormalPool;
    std::vector<std::shared_ptr<Model>> m_ModelPool;

    std::shared_ptr<Model_Textured> m_pModTEST = 0;
};