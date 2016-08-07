#include "MazeGame.h"
#include "Game.h"


MazeGame::MazeGame()
{
}


MazeGame::~MazeGame()
{
}

void MazeGame::Initialize( Graphics *pGraphics, Game * const pGame )
{
	m_pGame = pGame;
	m_pGraphics = pGraphics;

	m_Overlay.Initialize( *pGraphics );	
}

void MazeGame::UpdateFrame( const Input & InputRef, Camera *const pCamera )
{
	// MAKE CAMERA FOLLOW THE PLAYER
	// Get player position, offset camera, set camera position
	auto camOffset = m_player.GetWorldSpecs().position;
	camOffset.y += 30.f;
	pCamera->SetPosition( camOffset );

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
	// Initialize the board object
	m_board.Initialize( 9, 9 );

	// Initialize the test player actor
	m_player = Actor( {
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
	auto &gameView = m_pGame->GetGameView();
	gameView.Reset( m_pActorDrawList );

	// Move list of actors to board object
	m_board.SetCells( std::move( actorList ) );

	// Move player to start position
	auto startPos = m_board.GetStartPosition();
	m_player.Move( startPos );

	m_endReached = false;

}

void MazeGame::RenderFrame( const GameView & GameViewRef )
{
	m_Overlay.Render( *m_pGraphics );
	GameViewRef.UpdateView( m_pActorsMASTER ); // TODO: implement this new function

}

const TestBoard & MazeGame::GetBoard()
{
	return m_board;
}

