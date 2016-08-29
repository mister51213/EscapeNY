#include "Game_Maze.h"
#include "Game.h"
#include "Algorithm_Random.h"


void Game_Maze::Initialize(
	Graphics *pGraphics,
	class Game *const pGame,
	Camera *const pCamera )
{
	// Copy Game and Graphics pointers to member viarable
	m_pGame = pGame;
	m_pGraphics = pGraphics;
	m_pCamera = pCamera;
	

	m_pCamera->SetRotation( { 90.f, 0.f, 0.f } );
	
	m_scene.ambientColor = { 0.1f, 0.1f, 0.1f, 0.1f };
	m_scene.lightCount = 4;
	
	//m_Overlay.Initialize( *pGraphics );	
	reset();
}

void Game_Maze::UpdateScene( const Input &InputRef, Camera *const pCamera )
{
	m_player.GetInput( InputRef );

	// MAKE CAMERA FOLLOW THE PLAYER
	// Get player position, offset camera, set camera position
	auto camOffset = m_player.GetWorldSpecs().position;
	camOffset.y += 100.f;
	pCamera->SetPosition( camOffset );

	pCamera->GetInput( InputRef );

	for( int i = 0; i < m_pLights.size(); ++i )
	{
		auto *pLight = reinterpret_cast<Light_Spot*>( m_pLights[ i ].GetLight() );
		auto lightPos = pLight->GetPosition(); 
		auto lightDir = Normalize( m_player.GetPosition() - lightPos );
		pLight->SetDirection( lightDir.x, lightDir.y, lightDir.z );
	}
	// Pass input to overlay to check if player wants to regenerate the maze
	//m_Overlay.Update( InputRef );

	// Check if player has reached the end of the maze
	bool goalReached = m_board.HasReachedEnd( m_player );

	// If player has reached end of the maze and flag not set, 
	// set the flag that end has been reached
	if( !m_endReached )
	{
		if( goalReached )
		{
			m_endReached = goalReached;
			//m_Overlay.PlayerReachGoal();
		}
	}
	else
	{
		// If player has reached end and flag is set, ask for reset
		/*if( m_Overlay.WantsReset() )
		{
			reset();
		}*/
	}
}

void Game_Maze::RenderFrame( const GameView &GameViewRef )
{
	for( int i = 0; i < m_pLights.size(); ++i )
	{
		m_scene.lights[ i ] = m_pLights[ i ].GetLight()->GetLightBufferType();
	}
	GameViewRef.UpdateView( m_pActorDrawList, m_scene );

	// Overlay must be drawn last, since it draws directly to 
	// back buffer
	//m_Overlay.Render( *m_pGraphics );
}

void Game_Maze::reset()
{
	// Must clear draw list before using it during reset, 
	// clearing it when already empty doesn't hurt anything
	m_pActorDrawList.clear();

	// Initialize the board object
	m_board.Initialize( 9, 9, m_pGraphics );


	// Initialize the test player actor
	ModelSpecs_W wSpecs
	{
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f },
		{ .5f, .5f, .5f }
	};

	m_player = Actor_Player_Alt( wSpecs, SharkSkin, ModelSpecs_L() );

	// Create the maze and get the list of actors representing walls
	///////////////////////////////////////////////////
	// CODE FOR MAZE/LEVEL GEN
	/*Algorithm_Maze gen( this );
	auto actorList = gen.MakePattern( 0 );*/
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	// CODE FOR RANDOM LEVEL GEN
	Algorithm_Random gen( this );
	auto actorList = gen.MakePattern( 20 );
	///////////////////////////////////////////////////

	// Reserve space in draw list so pointers remain valid
	// Add additional space for actors not apart of a list
	m_pActorDrawList.reserve( actorList.size() + 2 );

	// Load draw list with actor pointers
	m_pActorDrawList.push_back( &m_player );

	for( auto &actor : actorList )
	{
		m_pActorDrawList.push_back( &actor );
	}

	// Board makes it's own model, so not passed to GameView::OnReset
	m_pActorDrawList.push_back( &m_board );

	// Move list of actors to board object
	m_board.SetCells( std::move( actorList ) );

	// Move player to start position
	auto startPos = m_board.GetStartPosition();
	m_player.SetPosition( startPos );

	m_pLights.resize( 4 );

	int count = 0;
	for( float j = -10.f; j <= 10.f; j += 20.f )
	{
		for( float i = -10.f; i <= 10.f; i += 20.f )
		{
			auto pos = XMFLOAT3{ i * 5.f, 20.f, j * 5.f };
			auto &light = m_pLights[ count ];
			light.Initialize( pos, { 0.f, 0.f, 0.f } );

			auto *pLight = dynamic_cast<Light_Spot*>( light.GetLight() );

			// ConeAngle can be between 10.f and 180.f...min/max'd in shader
			// The angle is divided by 180.f to normalize the value, then 
			// compared against the dot product of surface to light direction
			// and the light direction to test if it's in the cone of light.
			pLight->SetConeAngle( 1.f );
			switch( count )
			{
				case 0:
					pLight->SetColor( 1.f, 0.f, 0.f );
					break;
				case 1:
					pLight->SetColor( 0.f, 1.f, 0.f );
					break;
				case 2:
					pLight->SetColor( 0.f, 0.f, 1.f );
					break;
				case 3:
					pLight->SetColor( 1.f, 0.f, 1.f );
			}

			++count;
		}
	}


	m_endReached = false;
}

const TestBoard & Game_Maze::GetBoard()
{
	return m_board;
}

