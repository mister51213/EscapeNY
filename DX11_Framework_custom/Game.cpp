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
	UINT ScreenHeight,
	HWND WinHandle )
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

	m_board.Initialize( 9, 9 );
	// Setting position of camera to the start cell to be centered
	// over player
	auto localSpecs = m_board.GetLocalSpecs();
	auto startPos = m_board.GetStartPosition();
	m_pCamera->SetPosition( 
	{ 
		static_cast<float>(startPos.x) + 0.5f, 
		50.f, 
		static_cast<float>(startPos.y) + 0.5f
	} );
    ///////////////////////////////////////////////////
    // CODE FOR MAKING m_actorsSUB1 (ONE SUBSET OF ACTORS)
    m_numRows = 5; m_numColumns = 5; m_numZ = 5;
    m_numActors = m_numRows * m_numColumns * m_numZ;
    Algorithm_Grid3D alg;
    m_actorsSUB1 = makeActorSet(m_numActors, &alg);
    ///////////////////////////////////////////////////

    ///////////////////////////////////////////////////
    // CODE FOR MAKING m_actorsSUB2 (ONE SUBSET OF ACTORS)
    m_numActors = 100;
    Algorithm_Spiral3D alg2(this);
    m_actorsSUB2 = makeActorSet(m_numActors, &alg2);
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	// CODE FOR MAZE/LEVEL GEN
	Algorithm_Maze gen( this );
	m_actorsSUB3 = makeActorSet( 0, &gen );
	///////////////////////////////////////////////////


    makeActorsMASTER();

	// Pass all member pointers to GameObjects class so it can draw with them
    m_gObjects = 
        GameView(
            m_numActors, 
            m_pGraphics, 
            m_pDirect3D, 
            m_pCamera,
            WinHandle);

    m_gObjects.InitializeGameObjectsSystem(m_pActorsMASTER);

	return true;
}

void Game::GetInput( std::shared_ptr<Input> pInput )
{
	// TODO: Use global GetWorldMatrix() function instead 
	// TODO: of member WorldMatrixes for each model.
	// rotate objects
	if( pInput->IsKeyDown( VK_F3 ) )
	{
		RegenMap();
	}
	if( pInput->IsKeyDown( VK_SPACE ) )
	{
		//m_pModel1->Rotate( { 1.f,1.f,1.f } );
		//m_pModel2->Rotate( { 1.f,1.f,1.f } );
        for (auto& pActor: m_actorsSUB1)
        {
            pActor.Rotate({ 1.f,1.f,1.f });
        }
	}

	if( pInput->IsKeyDown( VK_CONTROL ) )
	{
		//m_pModel1->Rotate( { -0.1f,-5.f,-5.f } );
		//m_pModel2->Rotate( { -3.f,-0.3f,-5.f } );
        for (auto& pActor: m_actorsSUB1)
        {
            pActor.Rotate({ -1.f,-1.f,-1.f });
        }
	}

	// move objects
	if( pInput->IsKeyDown( VK_RIGHT ) )
	{
		//m_pModel1->Move( { .1f,0.f,0.f } );
		//m_pModel2->Move( { .9f,0.f,0.f } );
        for (auto& pActor: m_actorsSUB1)
        {
            pActor.Move({.9f,0.f,0.f });
        }
	}

	if( pInput->IsKeyDown( VK_LEFT ) )
	{
		//m_pModel1->Move( { -.1f,0.f,0.f } );
		//m_pModel2->Move( { -.9f,0.f,0.f } );
        for (auto& pActor: m_actorsSUB1)
        {
            pActor.Move({-.9f,0.f,0.f });
        }
	}

	if( pInput->IsKeyDown( VK_UP ) )
	{
		//m_pModel1->Move( { 0.f,.1f,0.f } );
		//m_pModel2->Move( { 0.f,.9f,0.f } );
        for (auto& pActor: m_actorsSUB1)
        {
            pActor.Move({ 0.f,.9f,0.f });
        }
	}

	if( pInput->IsKeyDown( VK_DOWN ) )
	{
		//m_pModel1->Move( { 0.f,-.1f,0.f } );
		//m_pModel2->Move( { 0.f,-.9f,0.f } );
        for (auto& pActor: m_actorsSUB1)
        {
            pActor.Move({ 0.f,-.9f,0.f });
        }
    }

	// move camera (FPS view)
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

bool Game::Frame()
{
	m_pGraphics->BeginScene();
	bool result = render();
	RETURN_IF_FALSE( result );
	m_pGraphics->EndScene();

	GetInput( m_pInput ); // Check input to modify object positioning.

	return true;
}

bool Game::render()
{
    // TODO: maybe initialize this in GameObjectsClass instead.
	// Generate the view matrix based on the camera's position.
	m_pCamera->Render();
    m_gObjects.UpdateView(m_pActorsMASTER); // TODO: implement this new function
 	m_Overlay.Render( *m_pGraphics );

	return true;
}

void Game::RegenMap()
{
	Algorithm_Maze gen( this );
	m_actorsSUB3 = makeActorSet( 0, &gen );

	int index = m_startOfMazeIndexInMasterList;
	for( auto &actor : m_actorsSUB3 )
	{
		m_pActorsMASTER[ index ] = &actor;
		++index;
	}

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
	m_startOfMazeIndexInMasterList = m_pActorsMASTER.size();
	for( auto &actor : m_actorsSUB3 )
	{
		m_pActorsMASTER.push_back( &actor );
	}
}

TestBoard & Game::GetBoard()
{
	return m_board;
}

// Make a set of similar actors based on algorithm
vector<Actor> Game::makeActorSet(int numActors, Algorithm* algorithm)
{
    return algorithm->MakePattern(numActors);
}
