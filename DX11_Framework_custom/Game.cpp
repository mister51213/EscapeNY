#include "Game.h"

using namespace DirectX;

Game::Game(std::shared_ptr<Input> pInput)
{
    m_pInput = pInput;
}

Game::~Game()
{
}

bool Game::Initialize(Graphics *pGraphics, UINT ScreenWidth, UINT ScreenHeight, HWND WinHandle)
{
    // Initialize input; clear key array - redundant?
    //m_pInput->Initialize();

    // Take a copy of the graphics and direct3d pointers
    m_pGraphics = pGraphics;
    m_pDirect3D = pGraphics->GetDirect3D();

    // Create the camera object.
    m_pCamera.reset(new Camera);
    bool result = m_pCamera != nullptr;
    RETURN_MESSAGE_IF_FALSE(result, L"Could not allocate memory for Camera.");

    // Set the initial position of the camera.
    result = m_pCamera->Initialize(
    m_camPos,					                // Position		
    m_camRotation, 						        // Rotation
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
    // Feed the Pimitive Maker w cube object
    ////////////////////////////////////////
    PrimitiveMaker primMaker;
    primMaker.CreateCube(m_modelOffset,{ 4.f, 4.f, 4.f });

    // Feed the Pimitive Maker 2 w cube object
    XMFLOAT3 mPos2 = m_modelOffset;
    mPos2.x += 4;
    mPos2.y += 2;
    mPos2.z += 7;
    PrimitiveMaker primMaker2;
    primMaker2.CreateCube(mPos2,{ 2.f, 2.f, 2.f });

    // Initialize model 1
	result = m_pModel->Initialize( primMaker, *m_pGraphics );
	RETURN_IF_FALSE( result );

    // Initialize model 2
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

	result = m_Overlay.Initialize( *m_pGraphics,ScreenWidth, ScreenHeight );
	RETURN_IF_FALSE( result );

	return true;
}

void Game::GetInput(std::shared_ptr<Input> pInput)
{
    // TODO: Use global GetWorldMatrix() function instead of member WorldMatrixes for each model.
    // rotate objects
    if (pInput->IsKeyDown(VK_SPACE))
    {
        m_pModel->Rotate({ 0.1f,0.1f,0.1f }, -.05f);
        m_pModel2->Rotate({ 0.1f,0.1f,0.1f }, .05f);
    }

    if (pInput->IsKeyDown(VK_CONTROL))
    {
        m_pModel->Rotate({ 0.1f,0.1f,0.1f }, 0.05f);
        m_pModel2->Rotate({ 0.1f,0.1f,0.1f }, -.05f);
    }

    // move objects
    if (pInput->IsKeyDown(VK_RIGHT))
    {
        m_pModel->Move({ .1f,0.f,0.f });
        m_pModel2->Move({ .2f,0.f,0.f });
    }

    if (pInput->IsKeyDown(VK_LEFT))
    {
        m_pModel->Move({ -.1f,0.f,0.f });
        m_pModel2->Move({ -.2f,0.f,0.f });
    }

    if (pInput->IsKeyDown(VK_UP))
    {
        m_pModel->Move({ 0.f,.1f,0.f });
        m_pModel2->Move({ 0.f,.2f,0.f });
    }

    if (pInput->IsKeyDown(VK_DOWN))
    {
        m_pModel->Move({ 0.f,-.1f,0.f });
        m_pModel2->Move({ 0.f,-.2f,0.f });
    }

    // move camera (FPS view)
    if (pInput->IsKeyDown(0x41)) // Left - A
    {
        m_pCamera->Move({ -1, 0, 0 });
    }

    if (pInput->IsKeyDown(0x53)) // Back - S
    {
        m_pCamera->Move({ 0, 0, -1 });
    }

    if (pInput->IsKeyDown(0x57)) // Fwd - W
    {
        m_pCamera->Move({ 0, 0, 1 });
    }

    if (pInput->IsKeyDown(0x44)) // Right - D
    {
        m_pCamera->Move({ 1, 0, 0 });
    }

    // rotate camera
        if (pInput->IsKeyDown(0x51)) // Left - Q
    {
        m_pCamera->Rotate({ 0, -1, 0 });
    }

    if (pInput->IsKeyDown(0x45)) // Right - E
    {
        m_pCamera->Rotate({ 0, 1, 0 });
    }
}

bool Game::Frame()
{
	m_pGraphics->BeginScene();

	bool result = render();
	RETURN_IF_FALSE( result );

	m_pGraphics->EndScene();

    // Check input to modify object positioning.
    GetInput(m_pInput);

	return true;
}

bool Game::render()
{
	// Generate the view matrix based on the camera's position.
	m_pCamera->Render();
	
	// Render the model using the color shader.
	bool result = m_pShader_Texture->Render( // sets shader parameters
		m_pDirect3D->GetDeviceContext(), 
		m_pModel->GetWorldMatrix(),
		m_pCamera->GetViewMatrix(),
		m_pCamera->GetProjectionMatrix(),
		m_pStoneTexture->GetTextureView() );

   	m_pGraphics->RenderModel( *m_pModel );
   	RETURN_IF_FALSE( result );

    // Render model 2
    	bool result2 = m_pShader_Texture->Render(
		m_pDirect3D->GetDeviceContext(), 
		m_pModel2->GetWorldMatrix(),
		m_pCamera->GetViewMatrix(),
		m_pCamera->GetProjectionMatrix(),
		m_pStoneTexture->GetTextureView() );
    
    //////////////////////////////////////////////////////////////////////
    // TODO: Change functionality here to draw MORE THAN ONE MODEL
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
    //////////////////////////////////////////////////////////////////////

// EXPERIMENT - render second model
    m_pGraphics->RenderModel( *m_pModel2 );
   	RETURN_IF_FALSE( result2 );


	auto overlayWorldMatrix = m_Overlay.GetWorldMatrix(*m_pCamera);

	m_pShader_Texture->Render(
		m_pDirect3D->GetDeviceContext(),
		overlayWorldMatrix,
		m_pCamera->GetViewMatrix(),
		m_pCamera->GetOrthoMatrix(),
		m_Overlay.GetResourceView()
	);

	m_Overlay.Render( *m_pGraphics );
	
	return true;
}