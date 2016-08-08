#include "MazeGame.h"
#include "Game.h"


MazeGame::MazeGame()
{
}


MazeGame::~MazeGame()
{
}

// Game and Camera are passed in as const pointer since we are wanting to 
// make a separate copy and we aren't using the constructor to initialize
// so can't use const &.  *const is kind of like a reference and const *const
// would be like const &.  It assures that the pointers passed in won't be 
// changed to point to something else, though the data that they point to 
// can be changed unless you use at least const *.
void MazeGame::Initialize( 
	Graphics *pGraphics,
	Game *const pGame,
	Camera *const pCamera )
{
	m_pGame = pGame;
	m_pGraphics = pGraphics;
	m_pCamera = pCamera;
	m_pCamera->SetRotation( { 90.f, 0.f, 0.f } );

	m_Overlay.Initialize( *pGraphics );	
	reset();
}

void MazeGame::UpdateFrame( const Input & InputRef )
{
	m_player.Update( InputRef );

	// MAKE CAMERA FOLLOW THE PLAYER
	// Get player position, offset camera, set camera position
	auto camOffset = m_player.GetWorldSpecs().position;
	camOffset.y += 30.f;
	m_pCamera->SetPosition( camOffset );

	// Pass input to overlay to check if player wants to regenerate the maze
	m_Overlay.Update( InputRef );

	// Check if player has reached the end of the maze
	bool goalReached = m_board.HasReachedEnd( m_player );

	// If player has reached end of the maze and flag not set, 
	// set the flag that end has been reached
	if( !m_endReached )
	{
		if( goalReached )
		{
			m_endReached = goalReached;
			m_Overlay.PlayerReachGoal();
		}
	}
	else
	{
		// If player has reached end and flag is set, ask for reset
		if( m_Overlay.WantsReset() )
		{
			reset();
		}
	}
}

void MazeGame::reset()
{
	// Must clear draw list before using it during reset, 
	// clearing it when already empty doesn't hurt anything
	m_pActorDrawList.clear();

	// Initialize the board object
	m_board.Initialize( 9, 9 );

	// Initialize the test player actor
	m_player = Actor_Player_Alt( {
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f },
		{ .5f, .5f, .5f } },
		eTexture::SharkSkin,
		ModelSpecs_L() );

	// Create the maze and get the list of actors representing walls
	///////////////////////////////////////////////////
	// CODE FOR MAZE/LEVEL GEN
	Algorithm_Maze gen( this );
	auto actorList = gen.MakePattern( 0 );
	///////////////////////////////////////////////////

	// Reserve space in draw list so pointers remain valid
	// Add additional space for actors not apart of a list
	m_pActorDrawList.reserve( actorList.size() + 1 );

	// Load draw list with actor pointers
	m_pActorDrawList.push_back( &m_player );
	for( auto &actor : actorList )
	{
		m_pActorDrawList.push_back( &actor );
	}

	// Load models for actors
	m_pGame->GetGameView().OnReset( m_pActorDrawList );

	// Move list of actors to board object
	m_board.SetCells( std::move( actorList ) );

	// Move player to start position
	auto startPos = m_board.GetStartPosition();
	m_player.Move( startPos );

	m_endReached = false;

}

void MazeGame::RenderFrame( const GameView & GameViewRef )
{
	GameViewRef.UpdateView( m_pActorDrawList );

	// Overlay must be drawn last, since it draws directly to 
	// back buffer
	m_Overlay.Render( *m_pGraphics );
}

const TestBoard & MazeGame::GetBoard()
{
	return m_board;
}

