#include "GameView.h"

GameView::GameView()
	:
	m_pGfx( nullptr ),
	m_pD3D( nullptr ),
	m_pCam( nullptr )
{}

// CODE_CHANGE: Initialize function no longer needs vector of Actor pointers
void GameView::Initialize( 
	ResourceManager *const pResource,
	Graphics* pGfx,
	Camera* pCam )
{
	m_pResource = pResource;
	m_pGfx = pGfx;
	m_pD3D = pGfx->GetDirect3D();
	m_pCam = pCam;
	initializeShader();
}

void GameView::UpdateView( const vector<Actor*>& actors )const
{
	for( const auto *actor : actors )
	{
		drawModel( *actor );
	}
}

// CODE_CHANGE: Renamed function to OnReset to be more clear of intent and
// made const, none of the functions called from here change the 
// state of GameView members.
void GameView::OnReset( const vector<Actor*>& pActors )const
{
	modelAllActors( pActors );
}

void GameView::initTexturePool()
{
	/*const int numTextures = 5;
	m_Textures.resize( numTextures );
	m_Textures[ AsphaltFresh ].Initialize( *m_pGfx, L"Textures\\fresh-black-asphalt-texture.jpg" );
	m_Textures[ AsphaltTGA ].Initialize( *m_pGfx, L"Textures\\asphalt.tga" );
	m_Textures[ AsphaltOld ].Initialize( *m_pGfx, L"Textures\\old-asphalt-texture.jpg" );
	m_Textures[ Water ].Initialize( *m_pGfx, L"Textures\\water3.jpg" );
	m_Textures[ SharkSkin ].Initialize( *m_pGfx, L"Textures\\sharkskin1.jpg" );*/
}

void GameView::initializeShader()
{
	m_shader_Texture.Initialize( m_pD3D->GetDevice() );
}

// CODE_CHANGE: Made const
void GameView::modelAllActors( const vector<Actor*>& actors )const
{
	for( int i = 0; i < actors.size(); i++ )
	{
		actors[ i ]->SetModel( makeModel( actors[ i ]->GetLocalSpecs() ) );
	}
}

// CODE_CHANGE: Made const
std::shared_ptr<Model> GameView::makeModel( ModelSpecs_L localSpecs, eModType Type )const
{
	std::shared_ptr<Model_Textured> pModel;
	pModel.reset( new Model_Textured );
	PrimitiveFactory primMaker;
	// TODO: Implement it so it can draw difft shapes based on eModType
	switch( Type )
	{
		case CUBE:
			primMaker.CreateCube( localSpecs );
			primMaker.CreateColor( 0.f, 0.f, 0.f, 1.f );
			break;
		case CUBE_TEXTURED:
			primMaker.CreateCube( localSpecs );
			break;
		case PLANE:
			primMaker.CreatePlane( localSpecs );
			break;
		case SPHERE:
			// primMaker.CreateSphere( localSpecs );
			break;
		case POLYGON:
			primMaker.CreateTriangle( localSpecs );
			break;
		case CUSTOM_MESH:
			//primMaker.CreateCustom( localSpecs );
			break;
		default:
			break;
	}
	pModel->Initialize( primMaker, *m_pGfx );
	return pModel;
}

void GameView::drawModel( const Actor & actor )const
{
	auto *const pContext = m_pD3D->GetDeviceContext();
	const auto &worldMatrix = GetWorldMatrix( actor.GetWorldSpecs() );
	const auto &viewMatrix = m_pCam->GetViewMatrix();
	const auto &projection = m_pCam->GetProjectionMatrix();
	auto *const pTextureView =
		m_pResource->GetTexture( actor.GetTexIndex() )->GetTextureView();
	
	m_shader_Texture.Render( 
		pContext, worldMatrix, viewMatrix, projection, pTextureView 
	);

	m_pGfx->RenderModel( *( actor.GetModel() ) );
}
