#include "GameView.h"

GameView::GameView()
	:
	m_pGfx( nullptr ),
	m_pD3D( nullptr ),
	m_pCam( nullptr )
{}

GameView::GameView( Graphics * pGfx, D3DGraphics * pD3D, std::shared_ptr<Camera>& pCam )
	:
	m_pGfx( pGfx ),
	m_pD3D( pD3D ),
	m_pCam( pCam )
{}

void GameView::Initialize()
{
    initModelPool();
	initTexturePool();
	initializeShader();
}

void GameView::UpdateView(const vector<Actor*>& actors, ILight* pLight) const
    {
        for each (Actor* actor in actors)
        {
            drawModel(*actor, pLight);
        }
    }

void GameView::drawModel( const Actor & actor, ILight* pLight ) const
{
    // UNTextured cube index is 0, so if > 0, use tex shader
    if (actor.GetModelType() > 0)
    {
        //// texture the actor
        //m_shader_Texture.Render(
        //    m_pD3D->GetDeviceContext(),
        //    GetWorldMatrix(actor.GetWorldSpecs()),
        //    m_pCam->GetViewMatrix(),
        //    m_pCam->GetProjectionMatrix(),
        //    (m_TexturePool[actor.GetTexIndex()]).GetTextureView());

        // lighting effects
        // TODO: Shader base should take a parameter that decides which effects to turn on and off,
        // TODO: which will be dictated by bool effectOn in FX_ struct
        //int indexCount = m_ModelPool[actor.GetModelType()]->GetIndexCount();
        //m_shader_Lighting.Render(
        //    m_pD3D->GetDeviceContext(),
        //    GetWorldMatrix(actor.GetWorldSpecs()),
        //    m_pCam->GetViewMatrix(),
        //    m_pCam->GetProjectionMatrix(),
        //    (m_TexturePool[actor.GetTexIndex()]).GetTextureView(),
        //    pLight);

        m_shader_LightingPT.Render(
            m_pD3D->GetDeviceContext(),
            GetWorldMatrix(actor.GetWorldSpecs()),
            m_pCam->GetViewMatrix(),
            m_pCam->GetProjectionMatrix(),
            (m_TexturePool[actor.GetTexIndex()]).GetTextureView(),
            pLight);
    }
    else
    {   
        // TODO: ALSO Update Color hlsl shaders to use lighting.
        // color the actor
        //m_shader_Color.Render(
        //    m_pD3D->GetDeviceContext(),
        //    GetWorldMatrix(actor.GetWorldSpecs()),
        //    m_pCam->GetViewMatrix(),
        //    m_pCam->GetProjectionMatrix(),
        //    NULL,
        //    pLight);

       // LIGHTING
        int indexCount = m_ModelPool[actor.GetModelType()]->GetIndexCount();
        m_shader_Lighting.Render(
            m_pD3D->GetDeviceContext(),
           // indexCount,
            GetWorldMatrix(actor.GetWorldSpecs()),
            m_pCam->GetViewMatrix(),
            m_pCam->GetProjectionMatrix(),
            (m_TexturePool[actor.GetTexIndex()]).GetTextureView(),
            pLight);
    }
    // TODO: there seems to be an error here with new FX* parameter
    m_pGfx->RenderModel(*(m_ModelPool[actor.GetModelType()]));
}

void GameView::initModelPool()
{
    char numModels = 10;
    ModelSpecs_L defaultSpecs;
    m_ModelPool.resize(numModels);

    PrimitiveFactory prim;

    prim.CreateCube(defaultSpecs);
    prim.CreateColor(1.f,0.f,0.f,.5f);
    m_ModelPool[CUBE].reset(new Model_Colored);
    m_ModelPool[CUBE]->Initialize(prim, *m_pGfx);

    prim.CreateCube(defaultSpecs);
    m_ModelPool[CUBE_TEXTURED].reset(new Model_Textured);
    m_ModelPool[CUBE_TEXTURED]->Initialize(prim, *m_pGfx);

    prim.CreatePlane(defaultSpecs);
    m_ModelPool[PLANE].reset(new Model_Textured);
    m_ModelPool[PLANE]->Initialize(prim, *m_pGfx);
    
    prim.CreateCube(defaultSpecs); // TODO: Change to CreateSphere
    m_ModelPool[SPHERE].reset(new Model_Textured);
    m_ModelPool[SPHERE]->Initialize(prim, *m_pGfx);

    prim.CreateTriangle(defaultSpecs);
    m_ModelPool[POLYGON].reset(new Model_Textured);
    m_ModelPool[POLYGON]->Initialize(prim, *m_pGfx);

    //prim.CreateMesh(L"Meshes\\Cube.txt");
    prim.CreateMesh(L"Meshes\\model.BinaryMesh");
    m_ModelPool[CUSTOM_MESH].reset(new Model_Textured);
    m_ModelPool[CUSTOM_MESH]->Initialize(prim, *m_pGfx);

    prim.CreateMesh(L"Meshes\\model2.BinaryMesh");
    m_ModelPool[CUSTOM_MESH2].reset(new Model_Textured);
    m_ModelPool[CUSTOM_MESH2]->Initialize(prim, *m_pGfx);

    prim.CreateMesh(L"Meshes\\model3.BinaryMesh");
    m_ModelPool[CUSTOM_MESH3].reset(new Model_Textured);
    m_ModelPool[CUSTOM_MESH3]->Initialize(prim, *m_pGfx);

    prim.CreateMesh(L"Meshes\\model4.BinaryMesh");
    m_ModelPool[CUSTOM_MESH4].reset(new Model_Textured);
    m_ModelPool[CUSTOM_MESH4]->Initialize(prim, *m_pGfx);
}

void GameView::initTexturePool()
{
	const int numTextures = 14;
	m_TexturePool.resize( numTextures );
	m_TexturePool[ AsphaltFresh ].Initialize( *m_pGfx, L"Textures\\fresh-black-asphalt-texture.jpg" );
	m_TexturePool[ AsphaltTGA ].Initialize( *m_pGfx, L"Textures\\asphalt.tga" );
	m_TexturePool[ AsphaltOld ].Initialize( *m_pGfx, L"Textures\\old-asphalt-texture.jpg" );
    m_TexturePool[ Water1 ].Initialize( *m_pGfx, L"Textures\\water1.jpg" );
    m_TexturePool[ Water2 ].Initialize( *m_pGfx, L"Textures\\water2.jpg" );
    m_TexturePool[ Water3 ].Initialize( *m_pGfx, L"Textures\\water3.jpg" );
	m_TexturePool[ Underwater1 ].Initialize( *m_pGfx, L"Textures\\underwater1.jpg" );
    m_TexturePool[ Underwater2 ].Initialize( *m_pGfx, L"Textures\\underwater2.jpg" );
    m_TexturePool[ Underwater3 ].Initialize( *m_pGfx, L"Textures\\underwater3.jpg" );
    m_TexturePool[ Underwater4 ].Initialize( *m_pGfx, L"Textures\\underwater4.jpg" );
    m_TexturePool[ Underwater5 ].Initialize( *m_pGfx, L"Textures\\underwater5.jpg" );
    m_TexturePool[ Underwater6 ].Initialize( *m_pGfx, L"Textures\\underwater6.jpg" );
    m_TexturePool[ Underwater7 ].Initialize( *m_pGfx, L"Textures\\underwater7.jpg" );
    m_TexturePool[ SharkSkin ].Initialize( *m_pGfx, L"Textures\\sharkskin1.jpg" );
}

void GameView::initializeShader()
{
	// m_shader_Texture.Initialize( m_pD3D->GetDevice() );
    m_shader_Color.Initialize( m_pD3D->GetDevice() );    
    m_shader_Lighting.Initialize(m_pD3D->GetDevice());
    m_shader_LightingPT.Initialize(m_pD3D->GetDevice());
}