#include "Game.h"
#include <time.h>

using namespace DirectX;

/*
	Going to need a few more interface classes for different game 
	components, like the overlay.  One overlay for each type of 
	screen like menus or game types since the overlay will be 
	handling specific interactions for the currently active game.

	The game types is more for while we are testing things out, but 
	the menus are a more important reason to do so.
*/

// To use a different game comment out the line with ( new MazeGame ) and 
// initialize the m_pActiveGame object to a new child of
// ISubGame; for example:
// m_pActiveGame(new TestGame) 
Game::Game( std::shared_ptr<Input> pInput )
	:
	m_pActiveGame(new MazeGame)
{
	m_pInput = pInput;
	srand( static_cast<unsigned int>( time( nullptr ) ) );	
}

Game::~Game() {}

bool Game::Initialize( Graphics *pGraphics,
	UINT ScreenWidth,
	UINT ScreenHeight)
{
	// Take a copy of the graphics and direct3d pointers
	m_pGraphics = pGraphics;
	m_pDirect3D = pGraphics->GetDirect3D();

	//////////////////////////////////
	// CREATE AND INITIALIZE CAMERA
	//////////////////////////////////
	m_pCamera.reset( new Camera );
	bool result = m_pCamera != nullptr;
	RETURN_MESSAGE_IF_FALSE( result, L"Could not allocate memory for Camera." );

	result = m_pCamera->Initialize(
		/*m_camPos*/{ 0.f,0.f,0.f },                // Position		
		/*m_camRotation*/{ 0.f,0.f,0.f },	        // Rotation
		{ ScreenWidth, ScreenHeight },				// Screen size
		{ SCREEN_NEAR, SCREEN_DEPTH } ); 		    // Screen clip depths
	RETURN_IF_FALSE( result );

	// Pass all member pointers to GameObjects class so it can draw with them
	m_GameView = GameView( m_pGraphics, m_pDirect3D, m_pCamera );
	m_GameView.InitializeGameObjectsSystem();
	
	//////////////////////////////
	// Initialize your sub-game //
	//////////////////////////////
	m_pActiveGame->Initialize( m_pGraphics, this, m_pCamera.get() );

	// CODE_CHANGE: Implement reset() if needed in ISubGame children.
	// It's not a virtual function of ISubGame, because it doesn't have
	// to be.  It's a private function so will only ever be called by
	// the child that implements it.  At least as far as I can tell.
	//reset();
	return true;
}

void Game::getInput( std::shared_ptr<Input> pInput )
{
	/*if( pInput->IsKeyDown( VK_F3 ) )
	{
		reset();
	}*/

	//const float playerSpeed = 0.2f;
	// move objects
	/*if( pInput->IsKeyDown( VK_RIGHT ) )
	{
		m_player.Move( { playerSpeed, 0.f, 0.f } );
	}
	else if( pInput->IsKeyDown( VK_LEFT ) )
	{
		m_player.Move( { -playerSpeed, 0.f, 0.f } );
	}

	if( pInput->IsKeyDown( VK_UP ) )
	{
		m_player.Move( { 0.f, 0.f, playerSpeed } );
	}
	else if( pInput->IsKeyDown( VK_DOWN ) )
	{
		m_player.Move( { 0.f, 0.f, -playerSpeed } );
    }

	if( pInput->IsKeyDown( VK_NEXT ) )
	{
		m_pCamera->Rotate( { -1.f, 0.f, 0.f } );
	}
	else if( pInput->IsKeyDown( VK_PRIOR ) )
	{
		m_pCamera->Rotate( { 1.f, 0.f, 0.f } );
	}	*/
}

// TODO: Make a list of Actor* ptrs, pass pointers to them to GameWorld.Update() function
// TODO: This function will get the world specs, draw all objects based on necessary info
// TODO: Use multiple inheritance for better efficiency 
// Ex.) one parent has health, the other has position

bool Game::Frame()
{
	updateGameObjects();

	m_pGraphics->BeginScene();
	render();
	m_pGraphics->EndScene();
	return true;
}

// If later we decide that the test games need to actually change the
// state of GameView, we can remove the const declarations.  Or, we
// can just store a copy of a GameView pointer in the the test game 
// children.
const GameView & Game::GetGameView()const
{
	return m_GameView;
}

bool Game::render()
{
    // TODO: maybe initialize this in GameObjectsClass instead.
	// Generate the view matrix based on the camera's position.
	m_pCamera->Render();
	m_pActiveGame->RenderFrame( m_GameView );

	return true;
}

//void Game::reset()
//{
//	/*m_actorsSUB1.clear();
//	m_actorsSUB2.clear();
//	m_actorsSUB3.clear();
//	m_actorsSUB4.clear();
//	m_pActorsMASTER.clear();*/
//
//	///////////////////////////////////////////////////////
//	// CODE FOR MAKING m_actorsSUB1 (ONE SUBSET OF ACTORS)
//	// These can be locally defined
//	/*const int numRows = 5, numColumns = 5, numZ = 5;
//	const int numActors = numRows * numColumns * numZ;
//	Algorithm_Grid3D alg;
//	m_actorsSUB1 = makeActorSet(numActors, &alg);*/
//	///////////////////////////////////////////////////////
//
//	////////////////////////////////////////////////////////
//	// CODE FOR MAKING m_actorsSUB2 (ONE SUBSET OF ACTORS)
//	// This can be locally defined, count will be stored in m_actorsSUB2.size()
//	/*const int numActors = 100;
//	Algorithm_Spiral3D alg2(this);
//	m_actorsSUB2 = makeActorSet(numActors, &alg2);*/
//	///////////////////////////////////////////////////
//
//    ///////////////////////////////////////////////////
//    ///////// FEED MASTER LIST of ACTORS //////////////
//    ///////////////////////////////////////////////////
//    // 1. Push a single PLAYER object into MASTER LIST
//    ///////////////////////////////////////////////////
//    //m_pActorsMASTER.push_back( &m_player );
//	///////////////////////////////////////////////////
//    // 2. Push ALL Actor subsets to MASTER LIST
//    ///////////////////////////////////////////////////
//    //makeActorsMASTER();
//
//    ///////////////////////////////////////////////////
//    //////////// RESET MASTER LIST //////////////////// 
//    ///////////////////////////////////////////////////
//	//m_GameView.Reset( m_pActorsMASTER );
//
//}

void Game::updateGameObjects()
{
	getInput( m_pInput ); // Check input to modify object positioning.
	m_pActiveGame->UpdateFrame( *m_pInput );
}

void Game::makeActorsMASTER()
{
    //// LOAD SUB1
    //for (Actor& actor: m_actorsSUB1)
    //{
    //    Actor* pActor = &actor;
    //    m_pActorsMASTER.push_back(pActor);
    //}
    // LOAD SUB2
    /*for (Actor& actor: m_actorsSUB2)
    {
        Actor* pActor = &actor;
        m_pActorsMASTER.push_back(pActor);
    }*/

	/*for( auto &actor : m_actorsSUB3 )
	{
		m_pActorsMASTER.push_back( &actor );
	}*/
}

// Make a set of similar actors based on algorithm
vector<Actor> Game::makeActorSet(int numActors, Algorithm* algorithm)
{
    return algorithm->MakePattern(numActors);
}
