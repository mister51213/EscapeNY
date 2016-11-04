#include "Game_Maze.h"
#include "Game.h"
#include "Algorithm_Random.h"
#include "MathUtils.h"

using namespace DirectX;

Game_Maze::Game_Maze( Input * pInput )
	:
	m_pInput( pInput )
{}

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

void Game_Maze::UpdateScene( const Input &InputRef, Camera *const pCamera, const Physics& refPhysics, const GameTimer& refTimer)
{
	UpdatePlayerState();
	UpdateCarState();
	UpdateMonsterState();
	
#ifdef NDEBUG
	const float detlatime = refTimer.SecondsPassed();
#else
	const float deltatime = .016f;
#endif

	m_player.Update( deltatime );

	// MAKE CAMERA FOLLOW THE PLAYER
	// Get player position, offset camera, set camera position
	const XMFLOAT3 &playerPosition = m_player.GetWorldSpecs().position;
	const XMFLOAT3 camOffset = { 0.f, 70.f, 0.f };

	pCamera->SetPosition( playerPosition + camOffset );

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
	//////////////////////////////////////////////////
	// Initialize the board object
	//////////////////////////////////////////////////
	m_board.Initialize( 9, 9, m_pGraphics );

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

	//////////////////////////////////////////////////
	// Initialize the player
	//////////////////////////////////////////////////
	// Initialize the test player actor
	ModelSpecs_W wSpecs
	{
		m_board.GetStartPosition(),
		{ 0.f, 0.f, 0.f },
		{ .5f, .5f, .5f }
	};

	m_player = Actor_Player_Alt( wSpecs, SharkSkin, m_pInput );

	//////////////////////////////////////////////////
	// Initialize the draw list
	//////////////////////////////////////////////////
	// Must clear draw list before using it during reset, 
	// clearing it when already empty doesn't hurt anything
	m_pActorDrawList.clear();

	// Reserve space in draw list so pointers remain valid
	// Add additional space for actors not apart of a list
	m_pActorDrawList.reserve( actorList.size() + 2 );

	// Load draw list with actor pointers
	m_pActorDrawList.push_back( &m_player );

	for( auto &actor : actorList )
	{
		m_pActorDrawList.push_back( &actor );
	}

	m_pActorDrawList.push_back( &m_board );

	// Move list of actors to board object
	m_board.SetCells( std::move( actorList ) );

	//////////////////////////////////////////////////
	// Initialize the lights
	//////////////////////////////////////////////////
	m_pLights.resize( 4 );

	int count = 0;
	Actor_Light *pActorLight = m_pLights.data();
	for( float j = -10.f; j <= 10.f; j += 20.f )
	{
		for( float i = -10.f; i <= 10.f; i += 20.f )
		{
			const XMFLOAT3 pos = XMFLOAT3{ i * 5.f, 20.f, j * 5.f };
			Actor_Light &light = *pActorLight;
			light.Initialize( pos, { 0.f, 0.f, 0.f } );

			Light_Spot *const pLight = dynamic_cast<Light_Spot*>( light.GetLight() );
			

			// ConeAngle can be between 1.f and 180.f...min/max'd in shader
			// The angle is divided by 180.f to normalize the value, then 
			// compared against the dot product of surface to light direction
			// and the light direction to test if it's in the cone of light.
			pLight->SetConeAngle( 15.f );
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
			++pActorLight;
		}
	}


	m_endReached = false;
}

const TestBoard & Game_Maze::GetBoard()
{
	return m_board;
}

void Game_Maze::UpdatePlayerState()
{
	Actor_Dynamic::eActorState state = m_player.GetState();
	if ( m_pInput->AnyPressed() )
	{
		switch ( state )
		{
			case Actor_Dynamic::PLAYER_IDLE:
				m_player.SetState( Actor_Dynamic::PLAYER_WALKING );
				break;
			case Actor_Dynamic::PLAYER_WALKING:
				break;
			case Actor_Dynamic::PLAYER_PUSHING:
				break;
			case Actor_Dynamic::PLAYER_QUICKTIME:
				// Mash keys, choose which keys to press
				break;
			case Actor_Dynamic::PLAYER_DROWNING:
				// Death animations
				break;
			case Actor_Dynamic::PLAYER_PINNED:
				// Death animations
				break;
			default:
				break;
		}
	}
	else
	{
		switch ( state )
		{
			case Actor_Dynamic::PLAYER_IDLE:
				break;
			case Actor_Dynamic::PLAYER_WALKING:
			case Actor_Dynamic::PLAYER_PUSHING:
				m_player.SetState( Actor_Dynamic::PLAYER_IDLE );
				break;
			case Actor_Dynamic::PLAYER_QUICKTIME:
				break;
			case Actor_Dynamic::PLAYER_DROWNING:
				break;
			case Actor_Dynamic::PLAYER_PINNED:
				break;
			default:
				break;
		}
	}
}

void Game_Maze::UpdateCarState()
{}

void Game_Maze::UpdateMonsterState()
{}

