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

	//////////////////////////////
	// Initialize your sub-game //
	//////////////////////////////
	m_pActiveGame->Initialize( m_pGraphics, this );

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

bool Game::render()
{
    // TODO: maybe initialize this in GameObjectsClass instead.
	// Generate the view matrix based on the camera's position.
	m_pActiveGame->RenderFrame();

	return true;
}

void Game::updateGameObjects()
{
	m_pActiveGame->UpdateFrame( *m_pInput );
}
