#include "Scene_Physics.h"
#include "Camera.h"
#include "D3DGraphics.h"
#include "DXUtils.h"
#include "Game.h"
#include "GameTimer.h"
#include "Graphics.h"
#include "Input.h"
#include "MathUtils.h"

using namespace DirectX;

void Scene_Physics::Initialize( Graphics * pGraphics, Game * const pGame, Camera * const pCamera )
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
		// light.Initialize( { x, y, z}, { 0.f, 0.f, 0.f } );

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

void Scene_Physics::reset()
{
	m_pActorsMASTER.clear();

	// MAP
	m_map = Actor_Stationary( 
	{ { 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 0.f },
	{ 600.f, 400.f, 600.f } }, waterSHALLOW, ModelSpecs_L(), SOME_EDIFICE );

    // LEFT BALL
	float radius1 = 50.0f;
    m_ball1 = Actor_Player(
	{ { -400.f, 300.f, -0.f },
	{ 0.f, 0.f, 0.f },
	{ radius1, radius1, radius1 } },
		eTexture::Aluminum, ModelSpecs_L(), SPHERE );
	m_ball1.m_attributes.radius = 50.0f; // NOTE technically radius in primitive factory is 1; then it gets scaled up by world specs!
	m_ball1.m_attributes.velocLin = { 200.0f,0.f,0.f  };
	//m_ball1.m_attributes.accelLin = { 0.f, -500.f, 0.f };
	m_ball1.m_attributes.mass = 100.f;

	// RIGHT BALL
	float radius2 = 50.0f;
	m_ball2 = Actor_Player(
    { { 400.f, 300.f, 0.f },
    { 0.f, 0.f, 0.f },
	{ radius2, radius2, radius2 } },		
		eTexture::SharkSkin, ModelSpecs_L(), SPHERE);
	m_ball2.m_attributes.radius = 50.0f; // NOTE technically radius in primitive factory is 1; then it gets scaled up by world specs!
	m_ball2.m_attributes.velocLin = { -200.0f,0.f,0.f  };
	//m_ball2.m_attributes.accelLin = { 0.f, -500.f, 0.f };
	m_ball2.m_attributes.mass = 50.f;

	// LOAD DRAW LIST
	m_pActorsMASTER.reserve( 3 );
	m_pActorsMASTER.push_back(&m_ball1);
	m_pActorsMASTER.push_back(&m_ball2);
    m_pActorsMASTER.push_back(&m_map);
}

void Scene_Physics::UpdateScene( Input & InputRef, Camera * const pCamera, const Physics & refPhysics, const GameTimer & refTimer )
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
	//m_physics.DoPhysics();

	// ACTOR MOVEMENT
	m_physics.UpdateActor( &m_ball1,tSinceLastFrame);
	m_physics.UpdateActor( &m_ball2,tSinceLastFrame);
	
	// CHECK COLLISIONS FOR BALLS
	CheckCollisions();

	// HANDLE ALL BALLS THAT COLLIDED
	if ( !m_pActorsOverlapping.empty() )
	{
		// TODO: REPLACE THIS WITH PHYSICS FUNCTIONALITY
		//m_pActorsOverlapping[ 0 ]->Rebound(m_pActorsOverlapping[1]);
		//m_pActorsOverlapping[ 1 ]->Rebound(m_pActorsOverlapping[0]);
	}
	m_pActorsOverlapping.clear(); // remove them from list

	// NOW RESUME COLLISION CHECKING ONCE BALLS HAVE BROKEN AWAY FROM EACH OTHER
	for ( auto & actor : m_pActorsMASTER )
	{
		if ( !Overlaps(&m_ball1, &m_ball2) )
		{
			dynamic_cast< Actor_Dynamic* >( actor )->ResumeCollisionChecking();
		}
	}

	// LIGHTS distributed among balls
	for ( int i = 0; i < m_lightsPerBall; i++ )
	{
		m_spotLights[i].SetLookat( m_ball1.GetPosition() );
	}
	for ( int i = m_lightsPerBall; i < m_numLights; i++ )
	{
		m_spotLights[ i ].SetLookat( m_ball2.GetPosition() );
	}

	/// INPUT ///
	GetInput(InputRef, tSinceLastFrame);
}

void Scene_Physics::CheckCollisions()
{
	if ( Overlaps( &m_ball1, &m_ball2 ))
	{
	if(!m_ball1.CollisionTurnedOff())
		m_pActorsOverlapping.push_back(&m_ball1);

	if(!m_ball2.CollisionTurnedOff())
		m_pActorsOverlapping.push_back(&m_ball2);
	}
}

/// GLOBAL INPUT ///
void Scene_Physics::GetInput(Input& pInput, float time)
{
	// push towards target 0,0,0
	XMFLOAT3 worldCenter = {0.f, 0.f, 0.f};

	// ball1 mass is 1
	XMFLOAT3 target1 = worldCenter - m_ball1.GetPosition();
	Normalize( target1 );
	target1 *= 300.f;

	// ball1 mass is 0.5
	XMFLOAT3 target2 = worldCenter - m_ball2.GetPosition();
	Normalize( target2 );
	target2 *= 300.f;

	if ( pInput.IsKeyDown( VK_SPACE ))
	{
		m_physics.ApplyForce( &m_ball1, target1, time );
		m_physics.ApplyForce( &m_ball2, target2, time );
	}
}

// TODO: REPLACE THIS WITH PHYSICS FUNCTIONALITY
bool Scene_Physics::Overlaps(Actor* pActor1, Actor* pActor2)
{
	float collisionDist = pActor1->GetAttributes().radius +
		pActor2->GetAttributes().radius;

	XMFLOAT3 distVector = pActor1->GetWorldSpecs().position -
		pActor2->GetWorldSpecs().position;
	float actorDistance = Magnitude( distVector );

	if ( abs( actorDistance ) <= collisionDist )
	{
		return true;
	}
	else
		return false;
}

void Scene_Physics::RenderFrame( const GameView & GameViewRef )
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

const SceneBufferType & Scene_Physics::GetSceneData() const
{
	// TODO: IMPLEMENT THIS
	return SceneBufferType();
}