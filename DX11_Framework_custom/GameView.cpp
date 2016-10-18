#include "GameView.h"
#include "Model_TexturedNM.h"

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
    initNormalMapPool();
	initializeShader();
}

// TODO: this should not directly take LightBufferType; it needs one level up (LightSpot type), 
// TODO: so it can pass world position info to shader for directional lighting
void GameView::UpdateView(const vector<Actor*>& actors, const SceneBufferType& SceneLights ) const
{ 
	
    /*bool result = m_activeShader.UpdateLightBuffer(
        m_pD3D->GetDeviceContext(),
        &(lightSet[0]));*/
	bool result = m_shader_nMap.UpdateLightBuffer( 
		m_pD3D->GetDeviceContext(), 
		SceneLights );
    // TODO: update the ShaderTEMPLATE to handle MULTPILE LIGHTS

    if (!result)
    {
        MessageBox(nullptr, L"Failed to update light buffer.", L"Problem...", MB_OK);
        PostQuitMessage(0);
        return;
    }

    // Transpose the matrices HERE now to adhere to the GPU column matrix convention
    MatrixBufferType transforms{};
    transforms.view = XMMatrixTranspose(m_pCam->GetViewMatrix());
    transforms.projection = XMMatrixTranspose(m_pCam->GetProjectionMatrix());

    for each (Actor* actor in actors)
    {
        drawModel(*actor, transforms);
    }
}

void GameView::drawModel( const Actor & actor, MatrixBufferType &Transforms ) const
{
  	// Prepare the graphics pipeline for this actor
	auto pContext = m_pD3D->GetDeviceContext();
	auto pTextureView = ( m_TexturePool[ actor.GetTexIndex() ] ).GetTextureView();
	Transforms.world = GetWorldMatrix( actor.GetWorldSpecs() );

	// Update the vertex shader's constant buffer
	bool result = m_shader_nMap.UpdateTransformBuffer( pContext, Transforms );
	//bool result = m_activeShader.UpdateTransformBuffer( pContext, Transforms );
	if( !result )
	{
		MessageBox( nullptr, L"Failed to update Matrix buffer.", L"Problem...", MB_OK );
		PostQuitMessage( 0 );
		return;
	}
	
    // TODO: pass an array of Textures instead here:
    
    ID3D11ShaderResourceView* texArray[2];

    texArray[0] = ( m_TexturePool[ actor.GetTexIndex() ] ).GetTextureView();
    texArray[1] = ( m_NormalPool[ actor.GetTexIndex() ] ).GetTextureView();

	// Set the shader and its resources
//	m_activeShader.Render( pContext, pTextureView );
    //m_activeShader.Render( pContext, texArray );
	m_shader_nMap.Render( pContext, texArray );

    // DRAW CALL
    m_pGfx->RenderModel(*(m_ModelPool[actor.GetModelType()]));
}

void GameView::initModelPool()
{
    char numModels = 11;
    ModelSpecs_L defaultSpecs;
    m_ModelPool.resize(numModels);

    PrimitiveFactory prim;
    prim.CreateCubeNM(defaultSpecs);
    prim.CreateColor(1.f,0.f,0.f,.5f);
    m_ModelPool[CUBE].reset(new Model_TexturedNM );
    m_ModelPool[CUBE]->Initialize(prim, *m_pGfx);

    prim.CreateCubeNM(defaultSpecs);
	prim.CreateColor( 1.f, 1.f, 1.f, 1.f );
    m_ModelPool[CUBE_TEXTURED].reset(new Model_TexturedNM );
    m_ModelPool[CUBE_TEXTURED]->Initialize(prim, *m_pGfx);

    prim.CreatePlaneNM(defaultSpecs);
    m_ModelPool[PLANE].reset(new Model_TexturedNM );
    m_ModelPool[PLANE]->Initialize(prim, *m_pGfx);
    
	prim.CreateSphereNM( defaultSpecs, 10.0f); // TODO: Change to CreateSphere
    m_ModelPool[SPHERE].reset(new Model_TexturedNM );
    m_ModelPool[SPHERE]->Initialize(prim, *m_pGfx);

    prim.CreateTriangle(defaultSpecs);
    m_ModelPool[POLYGON].reset(new Model_Textured);
    m_ModelPool[POLYGON]->Initialize(prim, *m_pGfx);

    //prim.CreateMesh(L"Meshes\\Cube.txt");
    prim.CreateMeshNM(L"Meshes\\model.BinaryMesh");
    m_ModelPool[CUSTOM_MESH].reset(new Model_TexturedNM );
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

	prim.CreateMeshNM( L"Meshes/cube_inverted.BinaryMesh" );
	m_ModelPool[ SOME_EDIFACE ].reset( new Model_TexturedNM );
	m_ModelPool[ SOME_EDIFACE ]->Initialize( prim, *m_pGfx );

    prim.CreateMeshNM( L"Meshes/car.BinaryMesh" );
	m_ModelPool[ CAR ].reset( new Model_TexturedNM );
	m_ModelPool[ CAR ]->Initialize( prim, *m_pGfx );

	//prim.CreateSphereNM( defaultSpecs, 10.0f );
	//prim.CreateColor( 1.f, 0.f, 0.f, 1.f );
	//m_ModelPool[ SPHERE ].reset( new Model_TexturedNM );
	//m_ModelPool[ SPHERE ]->Initialize( prim, *m_pGfx );
}

void GameView::initTexturePool()
{
	const int numTextures = 17;
	m_TexturePool.resize( numTextures );
	m_TexturePool[ AsphaltFresh ].Initialize( *m_pGfx, L"Textures\\asphaltFresh.jpg" );
	m_TexturePool[ AsphaltTGA ].Initialize( *m_pGfx, L"Textures\\asphalt.tga" );
	m_TexturePool[ AsphaltOld ].Initialize( *m_pGfx, L"Textures\\asphaltOld.jpg" );
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
    m_TexturePool[ SharkSkin ].Initialize( *m_pGfx, L"Textures\\sharkskin5.jpg" );
    m_TexturePool[ Aluminum ].Initialize( *m_pGfx, L"Textures\\aluminum.jpg" );
    m_TexturePool[ Lava ].Initialize( *m_pGfx, L"Textures\\lava5.jpg" );
    m_TexturePool[ Lava2 ].Initialize( *m_pGfx, L"Textures\\lava9.jpg" );
}

void GameView::initNormalMapPool()
{
    const int numTextures = 17;
	m_NormalPool.resize( numTextures );
	m_NormalPool[ AsphaltFresh ].Initialize( *m_pGfx, L"Textures\\asphaltFreshN.jpg" );
	m_NormalPool[ AsphaltTGA ].Initialize( *m_pGfx, L"Textures\\asphaltN.tga" );
	m_NormalPool[ AsphaltOld ].Initialize( *m_pGfx, L"Textures\\asphaltOldN.jpg" );
    m_NormalPool[ Water1 ].Initialize( *m_pGfx, L"Textures\\water1N.jpg" );
    m_NormalPool[ Water2 ].Initialize( *m_pGfx, L"Textures\\water2N.jpg" );
    m_NormalPool[ Water3 ].Initialize( *m_pGfx, L"Textures\\water3N.jpg" );
	m_NormalPool[ Underwater1 ].Initialize( *m_pGfx, L"Textures\\underwater1N.jpg" );
    m_NormalPool[ Underwater2 ].Initialize( *m_pGfx, L"Textures\\underwater2N.jpg" );
    m_NormalPool[ Underwater3 ].Initialize( *m_pGfx, L"Textures\\underwater3N.jpg" );
    m_NormalPool[ Underwater4 ].Initialize( *m_pGfx, L"Textures\\underwater4N.jpg" );
    m_NormalPool[ Underwater5 ].Initialize( *m_pGfx, L"Textures\\underwater5N.jpg" );
    m_NormalPool[ Underwater6 ].Initialize( *m_pGfx, L"Textures\\underwater6N.jpg" );
    m_NormalPool[ Underwater7 ].Initialize( *m_pGfx, L"Textures\\underwater7N.jpg" );
    m_NormalPool[ SharkSkin ].Initialize(*m_pGfx, L"Textures\\sharkskin5N.jpg");
    m_NormalPool[ Aluminum ].Initialize( *m_pGfx, L"Textures\\aluminumN.jpg" );
    m_NormalPool[ Lava ].Initialize( *m_pGfx, L"Textures\\lava5N.jpg" );
    m_NormalPool[ Lava2 ].Initialize( *m_pGfx, L"Textures\\lava9N.jpg" );
}

void GameView::initializeShader()
{
	//m_activeShader.Initialize( m_pD3D->GetDevice() );
	m_shader_nMap.Initialize( m_pD3D->GetDevice(), 10 );
}