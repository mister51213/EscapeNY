#include "Game.h"

using namespace DirectX;

Game::Game()
{}


Game::~Game()
{
}

bool Game::Initialize( Graphics *pGraphics, UINT ScreenWidth, UINT ScreenHeight, HWND WinHandle )
{
	// Take a copy of the graphics and direct3d pointers
	m_pGraphics = pGraphics;
	m_pDirect3D = pGraphics->GetDirect3D();

	// Create the camera object.
	m_pCamera.reset( new Camera );
	bool result = m_pCamera != nullptr;
	RETURN_MESSAGE_IF_FALSE( result, L"Could not allocate memory for Camera." );
	

	// Set the initial position of the camera.
	result = m_pCamera->Initialize(
	{0.0f, 0.0f, -5.0f},						// Position		
	{0.f, 0.f, 0.f}, 							// Rotation
	{ScreenWidth, ScreenHeight},				// Screen size
	{SCREEN_DEPTH, SCREEN_NEAR} 				// Screen clip depths
	);
	RETURN_MESSAGE_IF_FALSE( result, L"Could not initialize the camera object." );

	// Create the model object.
	m_pModel.reset( new Model );
	result = m_pModel != nullptr;
	RETURN_MESSAGE_IF_FALSE( result, L"Could not allocate memory for Model." );

	// Initialize the model object.
	result = m_pModel->Initialize( m_pDirect3D->GetDevice() );
	RETURN_MESSAGE_IF_FALSE( result, L"Could not initialize the model object." );

	// Create the color shader object.
	m_pColorShader.reset( new ColorShader );
	result = m_pColorShader != nullptr;
	RETURN_MESSAGE_IF_FALSE( result, L"Could not allocate memory for ColorShader." );

	// Initialize the color shader object.
	result = m_pColorShader->Initialize( m_pDirect3D->GetDevice(), WinHandle );
	RETURN_MESSAGE_IF_FALSE( result, L"Could not initialize color shader object." );

	return true;
}

bool Game::Frame()
{
	m_pGraphics->BeginScene();

	bool result = render();
	RETURN_IF_FALSE( result );

	m_pGraphics->EndScene();
	return true;
}

bool Game::render()
{
	// Generate the view matrix based on the camera's position.
	m_pCamera->Render();

	// Put the model vertex and index buffers on the graphics pipeline to
	// prepare them for drawing.
	m_pModel->Render( m_pDirect3D->GetDeviceContext() );
	
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	// Get the world, view, and projection matrices from the camera and model 
	// objects.
	m_pModel->GetWorldMatrix( worldMatrix );
	m_pCamera->GetViewMatrix( viewMatrix );
	m_pCamera->GetProjectionMatrix( projectionMatrix );

	// Render the model using the color shader.
	bool result = m_pColorShader->Render( m_pDirect3D->GetDeviceContext(),
		m_pModel->GetIndexCount(),
		worldMatrix,
		viewMatrix,
		projectionMatrix );

	RETURN_IF_FALSE( result );
	return true;
}
