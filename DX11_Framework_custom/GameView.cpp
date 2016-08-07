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
{
}

void GameView::InitializeGameObjectsSystem( const vector<Actor*>& actors )
{
	initTexturePool();
	initializeShader();
}

void GameView::UpdateView( const vector<Actor*>& actors )
{
	for( const auto *actor : actors )
	{
		drawModel( *actor );
	}
}

void GameView::Reset( const vector<Actor*>& pActors )
{
	modelAllActors( pActors );
}

void GameView::initTexturePool()
{
	const int numTextures = 5;
	m_Textures.resize( numTextures );
	m_Textures[ AsphaltFresh ].Initialize( *m_pGfx, L"Textures\\fresh-black-asphalt-texture.jpg" );
	m_Textures[ AsphaltTGA ].Initialize( *m_pGfx, L"Textures\\asphalt.tga" );
	m_Textures[ AsphaltOld ].Initialize( *m_pGfx, L"Textures\\old-asphalt-texture.jpg" );
	m_Textures[ Water ].Initialize( *m_pGfx, L"Textures\\water3.jpg" );
	m_Textures[ SharkSkin ].Initialize( *m_pGfx, L"Textures\\sharkskin1.jpg" );
}

void GameView::initializeShader()
{
	m_shader_Texture.Initialize( m_pD3D->GetDevice() );
}

void GameView::modelAllActors( const vector<Actor*>& actors )
{
	for( int i = 0; i < actors.size(); i++ )
	{
		actors[ i ]->SetModel( makeModel( actors[ i ]->GetLocalSpecs() ) );
	}
}

std::shared_ptr<Model> GameView::makeModel( ModelSpecs_L localSpecs, eModType Type )
{
	std::shared_ptr<Model_Textured> pModel;
	pModel.reset( new Model_Textured );
	PrimitiveFactory primMaker;
	// TODO: Implement it so it can draw difft shapes based on eModType
	primMaker.CreateCube( localSpecs );
	pModel->Initialize( primMaker, *m_pGfx );
	return pModel;
}

void GameView::drawModel( const Actor & actor )
{
	m_shader_Texture.Render(
		m_pD3D->GetDeviceContext(),
		GetWorldMatrix( actor.GetWorldSpecs() ),
		m_pCam->GetViewMatrix(),
		m_pCam->GetProjectionMatrix(),
		( m_Textures[ actor.GetTexIndex() ] ).GetTextureView() );

	m_pGfx->RenderModel( *( actor.GetModel() ) );
}
