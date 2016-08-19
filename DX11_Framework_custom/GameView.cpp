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

void GameView::UpdateView(
	const vector<Actor*>& actors, 
	const FX_Light& light) const
{
	/*Shader::LightBufferType lightBuffer;
	lightBuffer.diffuseColor = light.Color;
	lightBuffer.lightDirection = light.Direction;
	lightBuffer.padding = 0.f;
	
	bool result = m_tShader.UpdateLightBuffer( m_pD3D->GetDeviceContext(), lightBuffer );
	if( !result )
	{
		MessageBox( nullptr, L"Failed to update light buffer.", L"Problem...", MB_OK );
		PostQuitMessage( 0 );
		return;
	}

	Shader::MatrixBufferType transforms{};
	transforms.view = XMMatrixTranspose( m_pCam->GetViewMatrix() );
	transforms.projection = XMMatrixTranspose( m_pCam->GetProjectionMatrix() );

	for each ( Actor* actor in actors )
	{
		drawModel( *actor, transforms );
	}*/
}

void GameView::UpdateView( const vector<Actor*>& actors, const LightSpotBase & light ) const
{
	/*Shader::SpotLightBuffer spBuffer;
	spBuffer.m_colorAndIntensity = light.GetColor();
	spBuffer.m_colorAndIntensity.w = light.GetIntensity();
	spBuffer.m_direction = light.GetDirection();
	spBuffer.m_position = light.GetPosition();
	spBuffer.m_innerCone = light.GetInnerConeAngle();
	spBuffer.m_outerCone = light.GetOuterConeAngle();*/

	bool result = m_tShader.UpdateLightBuffer(
		m_pD3D->GetDeviceContext(), 
		light.GetDataBuffer());
	if( !result )
	{
		MessageBox( nullptr, L"Failed to update light buffer.", L"Problem...", MB_OK );
		PostQuitMessage( 0 );
		return;
	}

	Shader::MatrixBufferType transforms{};
	transforms.view = XMMatrixTranspose( m_pCam->GetViewMatrix() );
	transforms.projection = XMMatrixTranspose( m_pCam->GetProjectionMatrix() );

	for each ( Actor* actor in actors )
	{
		drawModel( *actor, transforms );
	}
}

void GameView::drawModel( 
	const Actor& actor,
	Shader::MatrixBufferType &Transforms ) const
{
	// Prepare the graphics pipeline for this actor
	auto pContext = m_pD3D->GetDeviceContext();
	auto pTextureView = ( m_TexturePool[ actor.GetTexIndex() ] ).GetTextureView();
	Transforms.world = GetWorldMatrix( actor.GetWorldSpecs() );

	// Update the vertex shader's constant buffer
	bool result = m_tShader.UpdateTransformBuffer( pContext, Transforms );
	if( !result )
	{
		MessageBox( nullptr, L"Failed to update Matrix buffer.", L"Problem...", MB_OK );
		PostQuitMessage( 0 );
		return;
	}
	
	// Set the shader and it's resources
	m_tShader.Render( pContext, pTextureView );
	
	// Render the actor
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
	m_TexturePool.resize( LASTTEXTURE );
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
	m_TexturePool[ FlatGray ].Initialize( *m_pGfx, L"Textures/gray.png" );
}

void GameView::initializeShader()
{
	m_tShader.Initialize( m_pD3D->GetDevice() );
	//m_shader.Initialize( m_pD3D->GetDevice() );
}