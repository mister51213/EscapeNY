#include "Game.h"
#include <time.h>

using namespace DirectX;

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

	bool result = m_camera.Initialize(
		{ ScreenWidth, ScreenHeight },				// Screen size
		{ SCREEN_NEAR, SCREEN_DEPTH } ); 		    // Screen clip depths
	RETURN_IF_FALSE( result );

	// Pass all member pointers to GameObjects class so it can draw with them
	m_GameView = GameView( m_pGraphics, m_pDirect3D, &m_camera );
	m_GameView.InitializeGameObjectsSystem();
	
	//////////////////////////////
	// Initialize your sub-game //
	//////////////////////////////
	m_pActiveGame->Initialize( m_pGraphics, this, &m_camera );

	return true;
}

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
	m_pActiveGame->RenderFrame( m_GameView );

	return true;
}

void Game::updateGameObjects()
{
	m_pActiveGame->UpdateFrame( *m_pInput );
}
