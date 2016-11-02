#include "Game.h"
#include <time.h>

using namespace DirectX;

Game::Game( std::shared_ptr<Input> pInput )
	:
	// change active game here
	//m_pActiveScene( new Game_FPS )
	//m_pActiveScene( new Scene )
	m_pActiveScene( new Game_Maze )
{
	m_pInput = pInput;
	srand( static_cast<unsigned int>( time( nullptr ) ) );
}

Game::~Game() {}

bool Game::Initialize( 
    Graphics *pGraphics,
	UINT ScreenWidth,
	UINT ScreenHeight)
{
	// Take a copy of the graphics and direct3d pointers
	m_pGraphics = pGraphics;
	m_pDirect3D = pGraphics->GetDirect3D();

	m_pCamera.reset( new Camera );
	bool result = m_pCamera != nullptr;
	RETURN_MESSAGE_IF_FALSE( result, L"Could not allocate memory for Camera." );
	
    result = m_pCamera->Initialize(
		{ 0.0f, 200.0f, 0.0f },					// Position		
		{ 90.f, 0.f, 0.f }, 						// Rotation
		{ ScreenWidth, ScreenHeight },				// Screen size
		{ g_screenNear, g_screenDepth } ); 		    // Screen clip depths
	RETURN_IF_FALSE( result );

	// Pass all member pointers to GameObjects class so it can draw with them
	m_GameView = GameView( m_pGraphics, m_pDirect3D, m_pCamera );
	m_GameView.Initialize();
    m_pActiveScene->Initialize(m_pGraphics, this, m_pCamera.get());

	return true;
}

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
	// Generate the view matrix based on the camera's position.
	m_pCamera->Render();
    m_pActiveScene->RenderFrame(m_GameView);
	return true;
}

void Game::updateGameObjects()
{
	// FOR TESTING PURPOSES, use constant time interval
	m_timer.Stop();
	
    m_pActiveScene->UpdateScene(*m_pInput, m_pCamera.get(), m_physics, m_timer);
	// phyics - move actual IMPLEMENTATIONS into main physics object

	m_timer.Start();
}