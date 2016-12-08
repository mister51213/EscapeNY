#include "Scene_Collision.h"
#include "Camera.h"
#include "D3DGraphics.h"
#include "DXUtils.h"
#include "Game.h"
#include "GameTimer.h"
#include "Graphics.h"
#include "Input.h"
#include "MathUtils.h"

using namespace DirectX;

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

void Scene_Collision::reset()
{
	m_pActorsMASTER.clear();

	// MAP
	/*m_map = Actor_NPC( 
	{ { 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 0.f },
	{ 600.f, 400.f, 600.f } }, Energy, ModelSpecs_L(), SOME_EDIFICE );*/
	m_map = Actor_Stationary( 
	{ { 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 0.f },
	{ 600.f, 400.f, 600.f } }, waterSHALLOW, ModelSpecs_L(), SOME_EDIFICE );

    // BALL 1
	float radius1 = 50.0f;
    m_ball1 = Actor_Player(
	{ { -400.f, 300.f, -0.f },
	{ 0.f, 0.f, 0.f },
	{ radius1, radius1, radius1 } },
		eTexture::Aluminum, ModelSpecs_L(), SPHERE );
	m_ball1.m_attributes.radius = 50.0f; // NOTE technically radius in primitive factory is 1; then it gets scaled up by world specs!

	// BALL 2
	float radius2 = 50.0f;
	m_ball2 = Actor_Player(
    { { 400.f, 300.f, 0.f },
    { 0.f, 0.f, 0.f },
	{ radius2, radius2, radius2 } },		
		eTexture::Underwater1, ModelSpecs_L(), SPHERE);
	m_ball2.m_attributes.radius = 50.0f; // NOTE technically radius in primitive factory is 1; then it gets scaled up by world specs!

	// LOAD DRAW LIST
	m_pActorsMASTER.reserve( 3 );
	m_pActorsMASTER.push_back(&m_ball1);
	m_pActorsMASTER.push_back(&m_ball2);
    m_pActorsMASTER.push_back(&m_map);
}

void Scene_Collision::UpdateScene( Input & InputRef, Camera * const pCamera, const Physics & refPhysics, const GameTimer & refTimer )
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

	// CHECK COLLISIONS FOR BALLS
	CheckCollisions();

	// HANDLE ALL BALLS THAT COLLIDED
	if ( !m_pActorsOverlapping.empty() )
	{
		//m_pActorsOverlapping[ 0 ]->ReboundDP(m_pActorsOverlapping[1]);
		//m_pActorsOverlapping[ 1 ]->ReboundDP(m_pActorsOverlapping[0]);
		m_pActorsOverlapping[ 0 ]->Rebound(m_pActorsOverlapping[1]);
		m_pActorsOverlapping[ 1 ]->Rebound(m_pActorsOverlapping[0]);
	}

	//for ( auto & actor : m_pActorsOverlapping )
	//{
		//actor->Stop();
		//actor->Rebound();
	//}

	m_pActorsOverlapping.clear(); // remove them from list

	// NOW RESUME COLLISION CHECKING ONCE BALLS HAVE BROKEN AWAY FROM EACH OTHER
	for ( auto & actor : m_pActorsMASTER )
	{
		// TODO: first check if the balls have cleared minimum safe distance before turning back on
		if ( !Overlaps(&m_ball1, &m_ball2) )
		{
			dynamic_cast< Actor_Dynamic* >( actor )->ResumeCollisionChecking();
		}
	}

	// TODO: 2 ISSUES
	/*
	1. For some reason it is resetting the target to 0,0,0 even AFTER the rebound function gets called;
	make sure that happens only once on keypress, and then the target can be overridden by the rebound function
	2. even though it temporarily turns off collision checking, it doesnt do it long enough for the balls
	to escape each others' check radii; it only turns if off for once cycle, which is not long enough to allow
	them to escape one another. 
	
	You should make it turn back on checking conditionally on whethere their
	DISTANCES have cleared the minimum safe distance instead.
	*/

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

void Scene_Collision::CheckCollisions()
{
	// TODO: ITERATE through all actors; compare one actor w every other actor in list;
	// Then do the same for all the other actors
	// TODO: add a second list to exclude the need for double comparisons
	// (if one actor found to overlap w another actor, then dont have to check 
	// vica versa when it comes time for that other actor to check for all OTHER actors)

	if ( Overlaps( &m_ball1, &m_ball2 ))
	{
	if(!m_ball1.CollisionOn())
		m_pActorsOverlapping.push_back(&m_ball1);

	if(!m_ball2.CollisionOn())
		m_pActorsOverlapping.push_back(&m_ball2);
	}

//FAILED ATTEMPT 2
//for (auto iterator = m_pActorsMASTER.begin(); iterator != m_pActorsMASTER.end(); ++iterator )
//{
//    auto & value = *iterator;
//	auto iteratorLimit = m_pActorsMASTER.end()++;
//
//	if ( iteratorLimit > )
//	{
//		bool overlaps = Overlaps( value, *( iterator++ ) );
//
//		if ( overlaps )
//		{
//			m_pActorsOverlapping.push_back( dynamic_cast< Actor_Dynamic* >( value ) );
//		}
//	}
//}
//FAILED ATTEMPT 1
	//for ( auto & actor : m_pActorsMASTER ) // NOTE what value is auto? iterator?
	//	// TODO: how to check if iterator is not at end
	//{
	//	if ( actor ) // check if actor is not null
	//	{
	//		if ( Overlaps( actor, actor++ ) ) // compare each actor with the next one in line
	//		{
	//			m_pActorsOverlapping.push_back( dynamic_cast<Actor_Dynamic*>(actor) );
	//			// TODO: make second list to eliminate redundant checking
	//		}
	//	}
	//}

}

bool Scene_Collision::Overlaps(Actor* pActor1, Actor* pActor2)
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
