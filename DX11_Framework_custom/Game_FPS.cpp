#include "Game_FPS.h"

void Game_FPS::Initialize(
    Graphics *pGraphics,
    class Game *const pGame,
    Camera *const pCamera) 
{
    	// Take a copy of the graphics and direct3d pointers
	m_pGraphics = pGraphics;
	m_pDirect3D = pGraphics->GetDirect3D();

	//////////////////////////////////
	// CREATE AND INITIALIZE CAMERA //
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

	// Use UpdateFrame to update actors, camera and anything else that 
	// needs updating.
void Game_FPS::UpdateFrame(const Input &InputRef)
{

}

	// Use RenderFrame to render the list of actors or other game objects
void Game_FPS::RenderFrame(const GameView &GameViewRef)
{


}