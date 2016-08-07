#include "Game.h"
#include "MazeGenerator.h"
#include <time.h>

using namespace DirectX;

Game::Game( std::shared_ptr<Input> pInput )
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
		m_camPos,					                // Position		
		m_camRotation, 						        // Rotation
		{ ScreenWidth, ScreenHeight },				// Screen size
		{ SCREEN_NEAR, SCREEN_DEPTH } ); 		    // Screen clip depths
	RETURN_IF_FALSE( result );

	result = m_Overlay.Initialize( *m_pGraphics );
	RETURN_IF_FALSE( result );

	// Pass all member pointers to GameObjects class so it can draw with them
	m_GameView = GameView( m_pGraphics, m_pDirect3D, m_pCamera );
	m_GameView.InitializeGameObjectsSystem( m_pActorsMASTER );

	reset();
	return true;
}

void Game::getInput( std::shared_ptr<Input> pInput )
{
    m_player.GetInput(pInput);
	// rotate objects
	//if( pInput->IsKeyDown( VK_F3 ) )
	//{
	//	reset();
	//}
	//if( pInput->IsKeyDown( VK_SPACE ) )
	//{
 //       for (auto& pActor: m_actorsSUB1)
 //       {
 //           pActor.Rotate({ 1.f,1.f,1.f });
 //       }
	//}

	if( pInput->IsKeyDown( VK_CONTROL ) )
	{
        for (auto& pActor: m_actorsSUB1)
        {
            pActor.Rotate({ -1.f,-1.f,-1.f });
        }
	}

	//const float playerSpeed = 0.2f;
	//// move objects
	//if( pInput->IsKeyDown( VK_RIGHT ) )
	//{
 //       for (auto& pActor: m_actorsSUB1)
 //       {
 //           pActor.Move({.9f,0.f,0.f });
 //       }
	//	//m_player.Move( { playerSpeed, 0.f, 0.f } );
	//}
	//else if( pInput->IsKeyDown( VK_LEFT ) )
	//{
 //       for (auto& pActor: m_actorsSUB1)
 //       {
 //           pActor.Move({-.9f,0.f,0.f });
 //       }
	//	//m_player.Move( { -playerSpeed, 0.f, 0.f } );
	//}

	//if( pInput->IsKeyDown( VK_UP ) )
	//{
 //       for (auto& pActor: m_actorsSUB1)
 //       {
 //           pActor.Move({ 0.f,.9f,0.f });
 //       }
	//	//m_player.Move( { 0.f, 0.f, playerSpeed } );
	//}
	//else if( pInput->IsKeyDown( VK_DOWN ) )
	//{
	//	//m_player.Move( { 0.f, 0.f, -playerSpeed } );
 //       for (auto& pActor: m_actorsSUB1)
 //       {
 //           pActor.Move({ 0.f,-.9f,0.f });
 //       }
 //   }

	if( pInput->IsKeyDown( VK_NEXT ) )
	{
		m_pCamera->Rotate( { -1.f, 0.f, 0.f } );
	}
	else if( pInput->IsKeyDown( VK_PRIOR ) )
	{
		m_pCamera->Rotate( { 1.f, 0.f, 0.f } );
	}
	 //move camera (FPS view)
	if( pInput->IsKeyDown( 0x41 ) ) // Left - A
	{
		m_pCamera->Move( { -1, 0, 0 } );
	}

	if( pInput->IsKeyDown( 0x53 ) ) // Back - S
	{
		m_pCamera->Move( { 0, 0, -1 } );
	}

	if( pInput->IsKeyDown( 0x57 ) ) // Fwd - W
	{
		m_pCamera->Move( { 0, 0, 1 } );
	}

	if( pInput->IsKeyDown( 0x44 ) ) // Right - D
	{
		m_pCamera->Move( { 1, 0, 0 } );
	}

	// rotate camera
	if( pInput->IsKeyDown( 0x51 ) ) // Left - Q
	{
		m_pCamera->Rotate( { 0, -1, 0 } );
	}

	if( pInput->IsKeyDown( 0x45 ) ) // Right - E
	{
		m_pCamera->Rotate( { 0, 1, 0 } );
	}
}

// TODO: Make a list of Actor* ptrs, pass pointers to them to GameWorld.Update() function
// TODO: This function will get the world specs, draw all objects based on necessary info
// TODO: Use multiple inheritance for better efficiency 
// Ex.) one parent has health, the other has position

//const TestBoard & Game::GetBoard()
//{
//	return m_board;
//}

bool Game::Frame()
{
	updateGameObjects();

	m_pGraphics->BeginScene();
	bool result = render();
	m_pGraphics->EndScene();
	return true;
}

bool Game::render()
{
    // TODO: maybe initialize this in GameObjectsClass instead.
	// Generate the view matrix based on the camera's position.
	m_pCamera->Render();
	m_GameView.UpdateView(m_pActorsMASTER); // TODO: implement this new function
 	m_Overlay.Render( *m_pGraphics );

	return true;
}
void Game::reset()
{
    m_actorsSUB1.clear();
    m_actorsSUB2.clear();
    m_actorsSUB3.clear();
    m_actorsSUB4.clear();
    m_pActorsMASTER.clear();
    //m_endReached = false;

    //m_board.Initialize( 9, 9 );
    //m_player = Actor_Player(
 //       m_pInput, 
 //       {{ 0.f, 0.f, 0.f },
    //	{ 0.f, 0.f, 0.f },
    //	{ .5f, .5f, .5f }}, 
 //       eTexture::SharkSkin,
 //       ModelSpecs_L());

    m_player = Actor(
        //pInput, 
    { { 0.f, 0.f, 0.f },
    { 0.f, 0.f, 0.f },
    { .5f, .5f, .5f } },
        eTexture::SharkSkin,
        ModelSpecs_L());

    ///////////////////////////////////////////////////////
    // MAKE m_actorsSUB1 (ONE SUBSET OF ACTORS)
    // These can be locally defined
    const int numRows = 5, numColumns = 5, numZ = 5;
    const int numActors = numRows * numColumns * numZ;
    Algorithm_Grid3D alg(m_pInput);
    //m_actorsSUB1 = makeActorSet(numActors, &alg);
    m_actorsSUB1 = alg.MakePatternNPC(numActors);

    ///////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////
    // MAKE m_actorsSUB2 (ONE SUBSET OF ACTORS)
    // This can be locally defined, count will be stored in m_actorsSUB2.size()
    const int numActors2 = 100;
    Algorithm_Spiral3D alg2(this, m_pInput);
    //m_actorsSUB2 = makeActorSet(numActors2, &alg2);
    m_actorsSUB2 = alg2.MakePatternNPC(numActors);

    ///////////////////////////////////////////////////

    ///////////////////////////////////////////////////
    // CODE FOR MAZE/LEVEL GEN
    /*Algorithm_Maze gen( this );
    m_actorsSUB3 = makeActorSet( 0, &gen );*/
    ///////////////////////////////////////////////////

    ///////////////////////////////////////////////////
    ///////// FEED MASTER LIST of ACTORS //////////////
    ///////////////////////////////////////////////////
    // 1. Push a single PLAYER object into MASTER LIST
    ///////////////////////////////////////////////////
    m_pActorsMASTER.push_back(&m_player);
    ///////////////////////////////////////////////////
    // 2. Push ALL Actor subsets to MASTER LIST
    ///////////////////////////////////////////////////
    makeActorsMASTER();

    ///////////////////////////////////////////////////
    //////////// RESET MASTER LIST //////////////////// 
    ///////////////////////////////////////////////////
    m_GameView.Reset(m_pActorsMASTER);

    ///////////////////////////////////////////////////
    //////////// MAZE FUNCTIONS ///////////////////////
    //m_board.SetCells( std::move( m_actorsSUB3 ) );
    //auto startPos = m_board.GetStartPosition();
 //   // Move player to start position
    //m_player.Move( startPos ); 
}

void Game::updateGameObjects()
{
	getInput( m_pInput ); // Check input to modify object positioning.

    // MAKE CAMERA FOLLOW THE PLAYER
    // Get player position, offset camera, set camera position
	/*auto camOffset = m_player.GetWorldSpecs().position;
	camOffset.y += 30.f;
	m_pCamera->SetPosition( camOffset );*/

    // CHECK IF PLAYER HAS REACHED GOAL and RESET if so
	/*m_Overlay.Update( *m_pInput );
	bool goalReached = m_board.HasReachedEnd( m_player );
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
		if( m_Overlay.WantsReset() )
		{
			reset();
		}
	}*/
}

void Game::makeActorsMASTER()
{
    // LOAD SUB1
    for (Actor& actor: m_actorsSUB1)
    {
        Actor* pActor = &actor;
        m_pActorsMASTER.push_back(pActor);
    }
    //LOAD SUB2
    for (Actor& actor: m_actorsSUB2)
    {
        Actor* pActor = &actor;
        m_pActorsMASTER.push_back(pActor);
    }

    // ADD The FULL MAZE map to the master list
	//for( auto &actor : m_actorsSUB3 )
	//{
	//	m_pActorsMASTER.push_back( &actor );
	//}
}

// Make a set of similar actors based on algorithm
vector<Actor> Game::makeActorSet(int numActors, Algorithm* algorithm)
{
    return algorithm->MakePattern(numActors);
}
