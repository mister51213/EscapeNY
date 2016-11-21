#include "Scene_Collision.h"

void Scene_Collision::Initialize( Graphics * pGraphics, Game * const pGame, Camera * const pCamera )
{
	// Take a copy of the graphics and direct3d pointers
	m_pGraphics = pGraphics;
	m_pDirect3D = pGraphics->GetDirect3D();
    m_pCamera = pCamera;
    m_Overlay.Initialize( *m_pGraphics );

    // INITIALIZE LIGHT
    m_spotLights.resize(m_numLights);
	for (Actor_Light& light: m_spotLights)
    {
		// Randomly create positions for new spot lights
		float x = static_cast<float>( rand() % 300 - 150 );
   		float y = static_cast<float>( rand() % 300 - 150 );
        float z = static_cast<float>( rand() % 300 - 150 );
//		light.Initialize( { x, y, z}, { 0.f, 0.f, 0.f } );

		// LOOKAT ball1
		light.Initialize( { x, y, z}, m_ball1.GetPosition() );
		
		// Randomly create colors for new spot lights
		float r = static_cast<float>(rand() % 100) * .01f;
		float g = static_cast<float>(rand() % 100) * .01f;
		float b = static_cast<float>(rand() % 100) * .01f;

		auto* pLight = dynamic_cast<Light_Spot*>( light.GetLight() );

		// Settings for newly created light.
		//pLight->SetColor( r, g, b );
		pLight->SetColor( .5f, .5f, .5f );
		pLight->SetConeAngle( 45.f );
		pLight->SetIntensity( 5.f );
    }

    m_lightSet.resize(m_numLights);

	reset();
}

void Scene_Collision::reset()
{
	m_pActorsMASTER.clear();

	// MAP
	m_map = Actor_NPC( 
	{ { 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 0.f },
	{ 600.f, 400.f, 600.f } }, Energy, ModelSpecs_L(), SOME_EDIFICE );

    // BALL 1
    m_ball1 = Actor_Player(
    { { 0.f, 100.f, -400.f },
    { 0.f, 0.f, 0.f },
    { .5f, .5f, .5f } }, eTexture::Bush, ModelSpecs_L(), SPHERE);

    // BALL 2
    m_ball2 = Actor_Player(
    { { 0.f, 100.f, 400.f },
    { 0.f, 0.f, 0.f },
    { .5f, .5f, .5f } }, eTexture::Bush, ModelSpecs_L(), SPHERE);

	// LOAD DRAW LIST
	m_pActorsMASTER.reserve( 3 );
	m_pActorsMASTER.push_back(&m_ball1);
	m_pActorsMASTER.push_back(&m_ball2);
    m_pActorsMASTER.push_back(&m_map);
}

void Scene_Collision::UpdateScene( const Input & InputRef, Camera * const pCamera, const Physics & refPhysics, const GameTimer & refTimer )
{
	// TIMER 
	#ifdef NDEBUG
	float tSinceLastFrame = refTimer.SecondsPassed();
	#else
	float tSinceLastFrame = 0.016f; // avg time btwn frames (60 FPS)
	#endif

    // INPUT
    m_pCamera->GetInput(InputRef);
    m_pCamera->GetMouseInput(InputRef);
    m_ball1.GetInput(InputRef);
    m_ball2.GetInput(InputRef);

	// PHYSICS
	//for each ( auto actor in m_pActorsMASTER )
	//{
	//	m_physics.DoPhysics( actor->GetAttributes(), tSinceLastFrame);
	//}

	// ACTOR MOVEMENT
	//m_ball1.SetState( Actor_Dynamic::HOMING );
	m_ball1.Update(tSinceLastFrame);
	//m_ball2.SetState( Actor_Dynamic::HOMING );
	m_ball2.Update(tSinceLastFrame);


	// LIGHTS distributed among balls

	//for ( Actor_Light& light : m_spotLights )
	for ( int i = 0; i < m_lightsPerBall; i++ )
	{
		m_spotLights[i].SetLookat( m_ball1.GetPosition() );
	}
	for ( int i = m_lightsPerBall; i < m_numLights; i++ )
	{
		m_spotLights[ i ].SetLookat( m_ball2.GetPosition() );
	}
}

void Scene_Collision::RenderFrame( const GameView & GameViewRef )
{
	// LIGHTS
	SceneBufferType scene{};
	scene.ambientColor = { .25f, .25f, .25f, .1f };
	scene.lightCount = min( m_spotLights.size(), MAX_SHADER_LIGHTS );
    for (int i = 0; i < m_spotLights.size(); i++)
    {
        scene.lights[i] = m_spotLights[i].GetLight()->GetLightBufferType();
    }

	// DRAW SCENE AND LIGHTS
    GameViewRef.UpdateView(m_pActorsMASTER, scene);
   	m_Overlay.Render( *m_pGraphics );
}

const SceneBufferType & Scene_Collision::GetSceneData() const
{
	// TODO: IMPLEMENT THIS
	return SceneBufferType();
}
