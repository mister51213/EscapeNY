#include "Game.h"

using namespace DirectX;

Game::Game( std::shared_ptr<Input> pInput )
{
	m_pInput = pInput;
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
		{ SCREEN_DEPTH, SCREEN_NEAR } ); 		    // Screen clip depths
	RETURN_IF_FALSE( result );

	//   // Model1
	//   m_pModel1.reset(new Model_Textured(m_modelOffset));
	//   result = m_pModel1 != nullptr;
	//   RETURN_MESSAGE_IF_FALSE(result, L"Could not allocate memory for Model.");
	//   PrimitiveFactory primMaker;
	//   primMaker.CreateCube({ 0.f, 0.f, 0.f }, { 5.f, 5.f, 5.f });
	//result = m_pModel1->Initialize( primMaker, *m_pGraphics );
	//RETURN_IF_FALSE( result );

	result = m_Overlay.Initialize( *m_pGraphics );
	RETURN_IF_FALSE( result );

	// Pass all member pointers to GameObjects class so it can draw with them
    m_gObjects = 
        GameWorld(
            10, 
            m_pGraphics, 
            m_pDirect3D, 
            m_pCamera,
            WinHandle);

    m_gObjects.InitializeGameObjectsSystem();

	return true;
}

void Game::GetInput( std::shared_ptr<Input> pInput )
{
	// TODO: Use global GetWorldMatrix() function instead 
	// TODO: of member WorldMatrixes for each model.
	// rotate objects
	if( pInput->IsKeyDown( VK_SPACE ) )
	{
		//m_pModel1->Rotate( { 1.f,1.f,1.f } );
		//m_pModel2->Rotate( { 1.f,1.f,1.f } );
        for each (auto pActor in m_actors)
        {
            pActor->Rotate({ 1.f,1.f,1.f });
        }
	}

	if( pInput->IsKeyDown( VK_CONTROL ) )
	{
		//m_pModel1->Rotate( { -0.1f,-5.f,-5.f } );
		//m_pModel2->Rotate( { -3.f,-0.3f,-5.f } );
        for each (auto pActor in m_actors)
        {
            pActor->Rotate({ -1.f,-1.f,-1.f });
        }
	}

	// move objects
	if( pInput->IsKeyDown( VK_RIGHT ) )
	{
		//m_pModel1->Move( { .1f,0.f,0.f } );
		//m_pModel2->Move( { .9f,0.f,0.f } );
        for each (auto pActor in m_actors)
        {
            pActor->Move({.9f,0.f,0.f });
        }
	}

	if( pInput->IsKeyDown( VK_LEFT ) )
	{
		//m_pModel1->Move( { -.1f,0.f,0.f } );
		//m_pModel2->Move( { -.9f,0.f,0.f } );
        for each (auto pActor in m_actors)
        {
            pActor->Move({-.9f,0.f,0.f });
        }
	}

	if( pInput->IsKeyDown( VK_UP ) )
	{
		//m_pModel1->Move( { 0.f,.1f,0.f } );
		//m_pModel2->Move( { 0.f,.9f,0.f } );
        for each (auto pActor in m_actors)
        {
            pActor->Move({ 0.f,.9f,0.f });
        }
	}

	if( pInput->IsKeyDown( VK_DOWN ) )
	{
		//m_pModel1->Move( { 0.f,-.1f,0.f } );
		//m_pModel2->Move( { 0.f,-.9f,0.f } );
        for each (auto pActor in m_actors)
        {
            pActor->Move({ 0.f,-.9f,0.f });
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

	//// MODEL1
	//bool result = m_pShader_Texture->Render( // sets shader parameters
	//    m_pDirect3D->GetDeviceContext(),
	//    GetWorldMatrix(m_pModel1->m_Position, ConvertToRadians(m_pModel1->m_Orientation), m_pModel1->m_Scale),
	//    m_pCamera->GetViewMatrix(),
	//    m_pCamera->GetProjectionMatrix(),
	//    m_pStoneTexture->GetTextureView());
	//m_pGraphics->RenderModel(*m_pModel1);
	//RETURN_IF_FALSE(result);

    m_gObjects.UpdateView(m_actors); // TODO: implement this new function
    m_gObjects.DrawAllModels();
	m_Overlay.Render( *m_pGraphics );

	return true;
}

void Game::makeAllActors()
{
    ModelSpecs_W specs = { { 0.f, 0.f, 0.f }, { 0.f,0.f,0.f }, { 1.f,1.f,1.f } };
    for (int i = 0; i < numActors; i++)
    {
        std::shared_ptr<Actor> pActor;
        m_actors.push_back(pActor.reset(new Actor(specs)));
        specs.position.x += 3;
        specs.position.y += 3;
        specs.orientation.z += 10;
    }
}
