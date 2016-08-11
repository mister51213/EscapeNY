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

void GameView::InitializeGameObjectsSystem( const vector<Actor*>& actors )
{
    initModelPool();
	initTexturePool();
	initializeShader();
}

void GameView::UpdateView( const vector<Actor*>& actors )
{
	for( const auto *actor : actors )
	{
		drawModel( *actor );
	}
    // TEST OUT SAMPLE MESH MODEL
    ModelSpecs_W wSpecs = {
        { 0.f, 0.f, 0.f },
        { 0.f,0.f,0.f },
        { 13.f, 13.f, 35.f } };
    std::shared_ptr<Input> pInput = 0;
    Actor_NPC aTest;
    aTest = Actor_NPC(pInput, wSpecs, Water, ModelSpecs_L(), CUBE);
    //aTest.SetModel(m_pModTEST); // TODO: change to actor.Initialize()
    drawModel(aTest);
}

void GameView::drawModel( const Actor & actor )
{
    // UNTextured cube index is 0, so if > 0, use tex shader
    if (actor.GetModelType()>0)
    {
        // texture the actor
        m_shader_Texture.Render(
            m_pD3D->GetDeviceContext(),
            GetWorldMatrix(actor.GetWorldSpecs()),
            m_pCam->GetViewMatrix(),
            m_pCam->GetProjectionMatrix(),
            (m_TexturePool[actor.GetTexIndex()]).GetTextureView());
    }
    else
    {   
        // color the actor
        m_shader_Color.Render(
            m_pD3D->GetDeviceContext(),
            GetWorldMatrix(actor.GetWorldSpecs()),
            m_pCam->GetViewMatrix(),
            m_pCam->GetProjectionMatrix());
    }

    m_pGfx->RenderModel(*(m_ModelPool[actor.GetModelType()]));

}

// TODO: implement model pool
void GameView::initModelPool()
{
    char numModels = 10;
    ModelSpecs_L defaultSpecs;
    m_ModelPool.resize(numModels);

    PrimitiveFactory prim;

    prim.CreateCube(defaultSpecs);
    prim.CreateColor(0.f,0.f,0.f,.5f);
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

    prim.CreateMesh(L"Meshes\\Cube.txt");
    m_ModelPool[CUSTOM_MESH].reset(new Model_Textured);
    m_ModelPool[CUSTOM_MESH]->Initialize(prim, *m_pGfx);

    // TEST
    //m_pModTEST.reset( new Model_Textured );
    //PrimitiveFactory prim;
    //prim.CreateMesh(L"Meshes\\Cube.txt");
    //m_pModTEST->Initialize( prim, *m_pGfx );
}

void GameView::initTexturePool()
{
	const int numTextures = 5;
	m_TexturePool.resize( numTextures );
	m_TexturePool[ AsphaltFresh ].Initialize( *m_pGfx, L"Textures\\fresh-black-asphalt-texture.jpg" );
	m_TexturePool[ AsphaltTGA ].Initialize( *m_pGfx, L"Textures\\asphalt.tga" );
	m_TexturePool[ AsphaltOld ].Initialize( *m_pGfx, L"Textures\\old-asphalt-texture.jpg" );
	m_TexturePool[ Water ].Initialize( *m_pGfx, L"Textures\\water3.jpg" );
	m_TexturePool[ SharkSkin ].Initialize( *m_pGfx, L"Textures\\sharkskin1.jpg" );
}

void GameView::initializeShader()
{
	m_shader_Texture.Initialize( m_pD3D->GetDevice() );
    m_shader_Color.Initialize( m_pD3D->GetDevice() );
}

//void GameView::modelAllActors( const vector<Actor*>& actors )
//{
//	for( int i = 0; i < actors.size(); i++ )
//	{
//		actors[ i ]->SetModel( makeModel( actors[ i ]->GetLocalSpecs() ) );
//	}
//    // TEST OUT MESH FUNCTION
//    	m_pModTEST.reset( new Model_Textured );
//    	PrimitiveFactory prim;
//        prim.CreateMesh(L"Meshes\\Cube.txt");
//        m_pModTEST->Initialize( prim, *m_pGfx );
//}

//std::shared_ptr<Model> GameView::makeModel( ModelSpecs_L localSpecs, eModType Type )
//{
//	std::shared_ptr<Model_Textured> pModel;
//	pModel.reset( new Model_Textured );
//	PrimitiveFactory primMaker;
//	// TODO: Implement drawing shapes based on eModType
//	primMaker.CreateCube( localSpecs );
//	pModel->Initialize( primMaker, *m_pGfx );
//	return pModel;
//}