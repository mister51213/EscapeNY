#include "Game.h"

using namespace DirectX;

Game::Game()
{}

Game::~Game()
{
}

bool Game::Initialize(Graphics *pGraphics, UINT ScreenWidth, UINT ScreenHeight, HWND WinHandle)
{
    // Take a copy of the graphics and direct3d pointers
    m_pGraphics = pGraphics;
    m_pDirect3D = pGraphics->GetDirect3D();

    // Create the camera object.
    m_pCamera.reset(new Camera);
    bool result = m_pCamera != nullptr;
    RETURN_MESSAGE_IF_FALSE(result, L"Could not allocate memory for Camera.");

    // Set the initial position of the camera.
    result = m_pCamera->Initialize(
    { -0.0f, 8.0f, -16.0f },					// Position		
    { 25.f, -5.f, 0.f }, 						// Rotation
    { ScreenWidth, ScreenHeight },				// Screen size
    { SCREEN_DEPTH, SCREEN_NEAR } 				// Screen clip depths
    );
    RETURN_IF_FALSE(result);

    // Create the model object.
    m_pModel.reset(new Model_Textured);
    result = m_pModel != nullptr;
    RETURN_MESSAGE_IF_FALSE(result, L"Could not allocate memory for Model.");

    // Create 2nd Model object
    m_pModel2.reset(new Model_Textured);
    result = m_pModel2 != nullptr;
    RETURN_MESSAGE_IF_FALSE(result, L"Could not allocate memory for Model.");

    // TODO: Must unify Initialize and Render functions for model if we want
    // to add responsive movement functionality in here

    ////////////////////////////////////////
    // Initialize the model object.
    ////////////////////////////////////////
    PrimitiveMaker primMaker;
    primMaker.CreateCube({ 1.f, 1.f, 1.f },{ 1.f, 1.f, 1.f });

    // Initialize 2nd Model object
    PrimitiveMaker primMaker2;
    primMaker2.CreateCube({ 3.f, 3.f, 3.f },{ 1.f, 1.f, 1.f });

    //primMaker.CreatePlane({ 0.f,0.f,0.f }, { 2.f,2.f });

	result = m_pModel->Initialize( primMaker, *m_pGraphics );
	RETURN_IF_FALSE( result );

	result = m_pModel2->Initialize( primMaker2, *m_pGraphics );
	RETURN_IF_FALSE( result );

	m_pShader_Texture.reset( new Shader_Texture );
	result = m_pShader_Texture != nullptr;
	RETURN_MESSAGE_IF_FALSE( result, L"Could not allocate memory for Shader_Texture." );
	//m_pColorShader.reset( new ColorShader );
	//result = m_pColorShader != nullptr;
	//RETURN_MESSAGE_IF_FALSE( result, L"Could not allocate memory for ColorShader." );

	//// Initialize the color shader object.
	result = m_pShader_Texture->Initialize( m_pDirect3D->GetDevice(), WinHandle, *m_pModel );
	RETURN_IF_FALSE( result );

	m_pStoneTexture.reset( new Texture );
	RETURN_MESSAGE_IF_FALSE( m_pStoneTexture != nullptr, L"Could not allocate memory for Texture." );

	result = m_pStoneTexture->Initialize( *m_pGraphics, L"Textures\\uncompressed_stone.tga" );
	RETURN_IF_FALSE( result );

	result = m_Overlay.Initialize( *m_pGraphics );
	RETURN_IF_FALSE( result );

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
	
	// Render the model using the color shader.
	bool result = m_pShader_Texture->Render(
		m_pDirect3D->GetDeviceContext(), 
		m_pModel->GetWorldMatrix(),
		m_pCamera->GetViewMatrix(),
		m_pCamera->GetProjectionMatrix(),
		m_pStoneTexture->GetTextureView() );


    // TODO: Point this at a DIFFERENT MODEL
    	bool result2 = m_pShader_Texture->Render(
		m_pDirect3D->GetDeviceContext(), 
		m_pModel2->GetWorldMatrix(),
		m_pCamera->GetViewMatrix(),
		m_pCamera->GetProjectionMatrix(),
		m_pStoneTexture->GetTextureView() );

    // TODO: Change functionality here to draw MORE THAN ONE MODEL
	// Put the model vertex and index buffers on the graphics pipeline to
	// prepare them for drawing.
	m_pGraphics->RenderModel( *m_pModel );

// EXPERIMENT - render second model
    m_pGraphics->RenderModel( *m_pModel2 );

	RETURN_IF_FALSE( result );

   	RETURN_IF_FALSE( result2 );

	m_pShader_Texture->Render(
		m_pDirect3D->GetDeviceContext(),
		DirectX::XMMatrixIdentity(),
		m_pCamera->GetViewMatrix(),
		m_pCamera->GetOrthoMatrix(),
		m_Overlay.GetResourceView()
	);

	m_Overlay.Render( *m_pGraphics );
	
	return true;
}