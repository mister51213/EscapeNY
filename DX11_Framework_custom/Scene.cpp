#include "Scene.h"
#include "Camera.h"
#include "GameView.h"
#include "Graphics.h"

void Scene::Initialize( Graphics * pGraphics, Game * const pGame, Camera * const pCamera, std::shared_ptr<std::vector<std::vector<DirectX::XMFLOAT3>>> pNormals)
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
		// LOOKAT PLAYER
		light.Initialize( { x, y, z}, m_player.GetPosition() );
		
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

void Scene::reset()
{
	m_pActorsMASTER.clear();

	// MAP
	m_map = Actor_NPC( 
	{ { 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 0.f },
	{ 300.f, 150.f, 300.f } }, Energy, ModelSpecs_L(), SOME_EDIFICE );

    // PLAYER
    m_player = Actor_Player(
    { { 0.f, 0.f, -150.f },
    { 0.f, 0.f, 0.f },
    { .5f, .5f, .5f } }, eTexture::Bush, ModelSpecs_L(), SPHERE);

	// LOAD DRAW LIST
	m_pActorsMASTER.reserve( 2 );
	m_pActorsMASTER.push_back(&m_player);
    m_pActorsMASTER.push_back(&m_map);
}

void Scene::UpdateScene( Input & InputRef, Camera * const pCamera, const Physics & refPhysics, const GameTimer & refTimer )
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
    m_player.GetInput(InputRef);

	// PHYSICS
	for each ( auto actor in m_pActorsMASTER )
	{
		m_physics.DoPhysics( actor->GetAttributes(), tSinceLastFrame);
	}

	// ACTOR MOVEMENT
	m_player.SetState( Actor_Dynamic::HOMING );
	m_player.Update(tSinceLastFrame);

	// LIGHTS FOLLOW ACTOR
	for ( Actor_Light& light : m_spotLights )
	{
		light.SetLookat( m_player.GetPosition() );
	}
}

void Scene::RenderFrame( const GameView & GameViewRef )
{
	// LIGHTS
	SceneBufferType scene{};
	scene.ambientColor = { .15f, .15f, .15f, .1f };
	scene.lightCount = min( m_spotLights.size(), MAX_SHADER_LIGHTS );
    for (int i = 0; i < m_spotLights.size(); i++)
    {
        scene.lights[i] = m_spotLights[i].GetLight()->GetLightBufferType();
    }

	// DRAW SCENE AND LIGHTS
    GameViewRef.UpdateView(m_pActorsMASTER, scene);
   	m_Overlay.Render( *m_pGraphics );
}

const SceneBufferType & Scene::GetSceneData() const
{
	// TODO: IMPLEMENT THIS
	return SceneBufferType();
}
