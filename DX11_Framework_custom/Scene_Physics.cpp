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

void Scene_Physics::Initialize( Graphics * pGraphics, 
								Game * const pGame, 
								Camera * const pCamera )
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

	// LEFT BOX
	float scale1 = 10.f;
    m_box1 = Actor_Player(
	{ { -400.f, 0.f, -0.f },
	{ 0.f, 0.f, 0.f },
	{ scale1, scale1, scale1 } },
		eTexture::Aluminum, ModelSpecs_L(), CUBE_TEXTURED );
	m_box1.m_attributes.mass = 25.f;

	// RIGHT BOX
	float scale2 = 10.f;
	m_box2 = Actor_Player(
    { { 400.f, 0.f, 0.f },
    { 0.f, 0.f, 0.f },
	{ scale2, scale2, scale2 } },		
		eTexture::SharkSkin, ModelSpecs_L(), CUBE_TEXTURED);
	m_box2.m_attributes.mass = 5.f;

	// LOAD DRAW LIST
	m_pActorsMASTER.push_back(&m_box1);
	m_pActorsMASTER.push_back(&m_box2);
    m_pActorsMASTER.push_back(&m_map);
}

void Scene_Physics::UpdateScene( 
	Input & InputRef, 
	Camera * const pCamera, 
	const Physics & refPhysics, 
	const GameTimer & refTimer )
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
    m_box1.GetInput(InputRef);
    m_box2.GetInput(InputRef);
	InputForces(InputRef, tSinceLastFrame);
	
	// ACTOR MOVEMENT
	m_box1.UpdateMotion(tSinceLastFrame);
	m_box2.UpdateMotion(tSinceLastFrame);
	// Stop actor accelerating after updating
	m_box1.StopAccelerating();
	m_box2.StopAccelerating();
	//TODO: this is a HACK; do it properly!

	// CHECK COLLISIONS FOR BALLS AND HANDLE COLLISIONS
	DoCollision();

	// Light up the balls
	Lighting();
}

/// GLOBAL INPUT ///
void Scene_Physics::InputForces(Input& pInput, float time)
{
	// get a force vector towards center of the world
	XMFLOAT3 worldCenter = {0.f, 0.f, 0.f};

	// box1 mass is 1
	XMFLOAT3 targetB1 = worldCenter - m_box1.GetPosition();
	Normalize( targetB1 );
	targetB1 *= 300.f; // force magnitude

	// box2 mass is 0.5
	XMFLOAT3 targetB2 = worldCenter - m_box2.GetPosition();
	Normalize( targetB2 );
	targetB2 *= 300.f; // force magnitude

	if ( pInput.IsKeyDown( VK_SPACE ))
	{
		m_physics.GenericForce( &m_box1, targetB1, time );
		m_physics.GenericForce( &m_box2, targetB2, time );
	}
}

/////////////////////////////////
/// CHECK COLLISION FUNCTIONS ///
/////////////////////////////////
void Scene_Physics::DoCollision()
{
	// Check for any collided objects in scene
	if ( AABBvsAABB( &m_box1, &m_box2 ))
	{
	if(m_box1.CollisionOn())
		m_pActorsOverlapping.push(&m_box1);

	if(m_box2.CollisionOn())
		m_pActorsOverlapping.push(&m_box2);
	}

	// HANDLE ALL OBJECTS THAT COLLIDED
	while ( !m_pActorsOverlapping.empty() )
	{
		m_pActorsOverlapping.front()->Stop();
		m_pActorsOverlapping.pop();
	}

	// NOW RESUME COLLISION CHECKING ONCE BALLS HAVE BROKEN AWAY FROM EACH OTHER
	for ( auto & actor : m_pActorsMASTER )
	{
		if ( !AABBvsAABB(&m_box1, &m_box2) ) // TODO: make it polymorphic for all shapes
		{
			dynamic_cast< Actor_Dynamic* >( actor )->ResumeCollisionChecking();
		}
	}
}

// check two axis-alligned bounding boxes
bool Scene_Physics::AABBvsAABB( Actor* pActor1, Actor* pActor2 )
{
	AABB box1 = pActor1->m_AABBox;
	AABB box2 = pActor2->m_AABBox;

	// Exit with no intersection if found separated along an axis
	if ( box1.m_max.x < box2.m_min.x || box1.m_min.x > box2.m_max.x ) return true;
	if ( box1.m_max.y < box2.m_min.y || box1.m_min.y > box2.m_max.y ) return true;
	if ( box1.m_max.z < box2.m_min.z || box1.m_min.z > box2.m_max.z ) return true;
 
  // No separating axis found, therefore there is at least one overlapping axis
	return false;
}

bool Scene_Physics::CircleVsCircle(Actor* pActor1, Actor* pActor2)
{
	float collisionDist = pActor1->GetAttributes().radius +
		pActor2->GetAttributes().radius;

	XMFLOAT3 distVector = pActor1->GetWorldSpecs().position -
		pActor2->GetWorldSpecs().position;
	float actorDistance = Magnitude( distVector );
	//TODO: optimize this by comparing a^2 + b^2 = c^2 and not calculating distance

	if ( abs( actorDistance ) <= collisionDist )
	{
		return true;
	}
	else
		return false;
}

bool Scene_Physics::CircleVsBox( Actor* pActor1, Actor* pActor2 )
{
		return true;
}

// non axis alligned bounding boxes
bool Scene_Physics::BoxVsBox( Actor* pActor1, Actor* pActor2 )
{
	return true;
}

///////////////
/// DRAWING ///
///////////////
void Scene_Physics::Lighting()
{
	// LIGHTS distributed among balls
	for ( int i = 0; i < m_lightsPerBall; i++ )
	{
		m_spotLights[i].SetLookat( m_box1.GetPosition() );
	}
	for ( int i = m_lightsPerBall; i < m_numLights; i++ )
	{
		m_spotLights[ i ].SetLookat( m_box2.GetPosition() );
	}
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

/// ACCESSOR ///
const SceneBufferType & Scene_Physics::GetSceneData() const
{
	// TODO: IMPLEMENT THIS
	return SceneBufferType();
}