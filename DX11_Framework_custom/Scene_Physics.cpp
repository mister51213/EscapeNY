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
using namespace std;

Scene_Physics::Scene_Physics( std::vector<std::vector<DirectX::XMFLOAT3>>& normPool )
{
	m_pNormals.reset(&normPool); // TODO: make this happen in INITIALIZE function so it happens after game view.
	// Now physics scene knows about normal maps of objects.
	// must make each object use these as templates, and then rescale individual version
	// to be held by each actor.
}

void Scene_Physics::Initialize( Graphics * pGraphics,
								Game * const pGame, 
								Camera * const pCamera,
								std::shared_ptr<std::vector<std::vector<DirectX::XMFLOAT3>>> pNormals)
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
		//light.Initialize( { x, y, z}, m_ball1.GetPosition() );
		
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

	m_pNormals = pNormals;

	reset();
}

void Scene_Physics::reset()
{
	// TODO: MUST INITIALIZE GAMEVIEW before this, 
	// so that we have the normal templates for actors to work with!

	//TODO: Also need to move normal / model pool into Game Object, not Gameview???

	m_pActorsMASTER.clear();
	m_collidables.clear();

	// LEFT BOX
	float scale1 = 100.f;
    m_collidables.push_back(
		Actor_Dynamic( 
			{ { -400.f, 0.f, 0.f },
			{ 0.f, 0.f, 0.f },
			{ scale1, scale1, scale1 } },
			eTexture::Aluminum, 
			ModelSpecs_L(), 
			CUBE_TEXTURED, 
			&(m_pNormals->at(CUBE_TEXTURED)))
	);

	m_collidables[0].m_attributes.mass = 5.f;

	// RIGHT BOX
	float scale2 = 200.f;
	m_collidables.push_back(Actor_Dynamic( 
    { { 400.f, 0.f, 0.f },
    { 0.f, 0.f, 0.f },
	{ scale2, scale2, scale2 } },		
		eTexture::SharkSkin, ModelSpecs_L(), CUBE_TEXTURED, &(m_pNormals->at(CUBE_TEXTURED))));
	m_collidables[1].m_attributes.mass = 10.f;

	// FLOOR
	m_collidables.push_back(Actor_Dynamic( 
	{ { 0.f, -200.f, 0.f },
	{ 0.f, 0.f, 0.f },
	{ 1500.f, 20.f, 1500.f } }, Rock, ModelSpecs_L(), CUBE_TEXTURED, &(m_pNormals->at(CUBE_TEXTURED))));
	m_collidables[2].m_attributes.mass = 10000.f;

	// RIGHT WALL
	m_collidables.push_back(Actor_Dynamic( 
	{ { 750.f, 230.f, 0.f },
	{ 0.f, 0.f, 0.f },
	{ scale2, 500.f, 1500.f } }, waterSHALLOW, ModelSpecs_L(), CUBE_TEXTURED, &(m_pNormals->at(CUBE_TEXTURED))));
	m_collidables[3].m_attributes.mass = 5000.f;

	// LEFT WALL
	m_collidables.push_back(Actor_Dynamic( 
	{ { -750.f, 230.f, 0.f },
	{ 0.f, 0.f, 0.f },
	{ scale2, 500.f, 1500.f } }, waterSHALLOW, ModelSpecs_L(), CUBE_TEXTURED, &(m_pNormals->at(CUBE_TEXTURED))));
	m_collidables[4].m_attributes.mass = 100.f;

	// LOAD DRAW LIST
	for ( int i = m_collidables.size() - 1; i >= 0; i-- )
	{
		m_pActorsMASTER.push_back(&(m_collidables[i]) );
	}
	
	/*
	m_pActorsMASTER.push_back(&m_box1);
	m_pActorsMASTER.push_back(&m_box2);
    m_pActorsMASTER.push_back(&m_map);*/

	//m_collidableObjects[ 0 ] = &m_box1;
	//m_collidableObjects[ 1 ] = &m_box2;
	//m_collidableObjects[ 2 ] = &m_map;
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
	InputForces(InputRef, tSinceLastFrame);

	// Gravity
	m_physics.Force_Steady( &m_collidables[0], { 0.f, -60000.f, 0.f }, tSinceLastFrame );
	m_physics.Force_Steady(&m_collidables[1], { 0.f, -60000.f, 0.f }, tSinceLastFrame);

	// ACTOR MOVEMENT
	for ( int i = 0; i < m_collidables.size(); i++ )
	{
		m_collidables[i].UpdateMotion(tSinceLastFrame);
		m_collidables[i].StopAccelerating();
	}

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

	XMFLOAT3 targetB1 = worldCenter - m_collidables[0].GetPosition();
	Normalize( targetB1 );
	targetB1 *= 300.f; // force magnitude

	XMFLOAT3 targetB2 = worldCenter - m_collidables[1].GetPosition();
	Normalize( targetB2 );
	targetB2 *= 300.f; // force magnitude

	if ( pInput.IsKeyDown( VK_LEFT ))
	{
		m_physics.Force_Steady( &m_collidables[0], { 120000.f, 0.f, 0.f }, time ); // force on left
		m_physics.Force_Steady( &m_collidables[1], { -240000.f, 0.f, 0.f }, time ); // force on right
	}
	if ( pInput.IsKeyDown( VK_RIGHT ))
	{
		m_physics.Force_Steady( &m_collidables[0], { -120000.f, 0.f, 0.f }, time ); // force on left
		m_physics.Force_Steady( &m_collidables[1], { 240000.f, 0.f, 0.f }, time ); // force on right
	}
	if ( pInput.IsKeyDown( VK_UP ))
	{
		m_physics.Force_Steady( &m_collidables[0], { 0.f, 120000.f, 0.f }, time ); // force on left
		m_physics.Force_Steady( &m_collidables[1], { 0.f, 120000.f, 0.f }, time ); //// force on right
	}
	if ( pInput.IsKeyDown( VK_DOWN ))
	{
		m_physics.Force_Steady( &m_collidables[0], { 0.f, -120000.f, 0.f }, time ); // force on left
		m_physics.Force_Steady( &m_collidables[1], { 0.f, -120000.f, 0.f }, time ); // force on right
	}
	if ( pInput.IsKeyDown( 'R' ))
	{

		reset();
	}
}

/////////////////////////////////
/// CHECK COLLISION FUNCTIONS ///
/////////////////////////////////
void Scene_Physics::DoCollision()
{
	//for ( int i = 0; i < m_numCollidables; i++ )
	//{
	//	Actor_Dynamic* pBox1 = m_collidableObjects[ i ];
	//	Actor_Dynamic* pBox2 = NULL;
	//	// check if there's one more actor in the list to add as partner
	//	int next = i + 1;
	//	if (next < m_numCollidables)
	//	{
	//	pBox2 = m_collidableObjects[ next ];
	//	}
	//	else // otherwise test it w first actor in list
	//	{
	//	pBox2 = m_collidableObjects[ 0 ];
	//	}
	//	// check collision
	//	if ( AABBvsAABB( pBox1, pBox2 ) )
	//	{
	//		if ( pBox1->CollisionOn() && pBox2->CollisionOn() )
	//		{
	//			pBox1->m_pCollision_partner = pBox2;
	//			m_pActorsOverlapping.push( pBox1 );
	//		}
	//	}
	//}

	eCollisionDir eDir;

	// double iterator loop for collision checking

	//TODO: just make the inner iterator start looping at iter1++, 
	//TODO: and only loop until m_collidables.end()-- because last iteration is redundant

		vector<Actor_Dynamic>::iterator skipMe;

		for ( vector<Actor_Dynamic>::iterator iter1 = m_collidables.begin(); iter1 != m_collidables.end(); ++iter1 )
		{
			if ( iter1 != m_collidables.begin() )
			{
				skipMe = iter1;
				--skipMe;
			}

			for ( std::vector<Actor_Dynamic>::iterator iter2 = m_collidables.begin(); iter2 != m_collidables.end(); ++iter2 )
			{
				if ( iter1 == iter2 )
					continue;

				if ( iter1 != m_collidables.begin() )
				{
					if ( iter2 <= skipMe ) // The <= is the key!
						continue;
				}

				//if ( AABBvsAABB( iter1, iter2 ) )
				if ( GetCollisionNormal( iter1, iter2, eDir) )	
				{
					iter1->ReboundWith( iter2, eDir );
				}
			}
		}

//	// If the two objects overlap, add one to list and give it awareness of the other
//	for ( UINT64 i = 0; i < m_pActorsMASTER.size(); i++ )
//	{
//		Actor_Dynamic* pBox1 = ( Actor_Dynamic* )m_pActorsMASTER[ i ];
//		
//		Actor_Dynamic* pBox2 = NULL;
//		int size = m_pActorsMASTER.size();
//		//if (i++ != size--)
//		//{
//		pBox2 = ( Actor_Dynamic* )m_pActorsMASTER[ i++ ];
////		}
//
//		if ( AABBvsAABB( pBox1, pBox2 ) )
//		{
//			if ( pBox1->CollisionOn() && pBox2->CollisionOn() )
//			{
//				m_pActorsOverlapping.push( pBox1 );
//				m_box1.m_pCollision_partner = pBox2;
//			}
//		}
//	}	
	//if ( AABBvsAABB( &m_box1, &m_box2 ))
	//{ // TODO: check if already added as a partner force
	//	if ( m_box1.CollisionOn() && m_box2.CollisionOn() )
	//	{
	//		m_pActorsOverlapping.push( &m_box1 );
	//		m_box1.m_pCollision_partner = &m_box2;
	//	}
	//}
	//if ( AABBvsAABB( &m_box1, &m_map ))
	//{ // TODO: check if already added as a partner force
	//	if ( m_box1.CollisionOn() && m_map.CollisionOn() )
	//	{
	//		m_box1.ReboundX1();
	//	}
	//}
	//	if ( AABBvsAABB( &m_box2, &m_map ))
	//{ // TODO: check if already added as a partner force
	//	if ( m_box2.CollisionOn() && m_map.CollisionOn() )
	//	{
	//		m_box2.ReboundX1();
	//	}
	//}
	// TODO: how to implement collision with many objects??
	// HANDLE ALL OBJECTS THAT COLLIDED
	//while ( !m_pActorsOverlapping.empty() )
	//{
	//	//m_pActorsOverlapping.front()->Stop();
	//	// handle both the actor and its partner
	//	Actor_Dynamic* pActor = m_pActorsOverlapping.front();
	//	Actor_Dynamic* pPartner = pActor->m_pCollision_partner;
	//	/*XMFLOAT3 impulse = */pActor->ReboundX2();
	//	///*XMFLOAT3 partnerImpulse = */pPartner->GetReboundForce(pActor); // 
	//	// TODO: why is partnerImpulse getting set to 000??? not getting calculated or initialized properly
	//		// ANSWER - because it has no partner force of its own. these two things should be processed within actors local function.
	//	//m_physics.Force_Collision( pActor, impulse );
	//	//m_physics.Force_Collision( pPartner, partnerImpulse );
	//	m_pActorsOverlapping.pop();
	//}
	// NOW RESUME COLLISION CHECKING ONCE BALLS HAVE BROKEN AWAY FROM EACH OTHER
//for ( vector<Actor_Dynamic>::iterator iter1 = m_collidables.begin(); iter1 != m_collidables.end(); ++iter1 )
//{
//	for ( std::vector<Actor_Dynamic>::iterator iter2 = m_collidables.begin(); iter2 != m_collidables.end(); ++iter2 )
//	{
//		if ( iter1 == iter2 )
//		{
//			// don't compare for collision against itself
//			continue;
//		}
//		if ( !AABBvsAABB( iter1, iter2 ) ) // TODO: make it polymorphic for all shapes
//		{
//			iter1->ResumeCollision();
//			iter2->ResumeCollision();
//		}
//	}
//}
}

// TODO: move this INSIDE the AABB struct
bool Scene_Physics::AABBvsAABB( vector<Actor_Dynamic>::iterator pActor1, vector<Actor_Dynamic>::iterator pActor2 )
{
 	AABB box1 = pActor1->m_AABBox;
	AABB box2 = pActor2->m_AABBox;

	// A SEPARATING AXIS WAS FOUND, so exit with no intersection
	if ( box1.m_max.x < box2.m_min.x || box1.m_min.x > box2.m_max.x ) return false;
	if ( box1.m_max.y < box2.m_min.y || box1.m_min.y > box2.m_max.y ) return false;
	if ( box1.m_max.z < box2.m_min.z || box1.m_min.z > box2.m_max.z ) return false;
 
  // No separating axis found, therefore there is at least one overlapping axis
	return true;
}

// call this to check collision AND see which face collided w which
// FALSE means no collision. TRUE means collision.
// TODO: must revert of old way of doing it, must check ALL THREE dimensions every time!
bool Scene_Physics::GetCollisionNormal( vector<Actor_Dynamic>::iterator pActor1, vector<Actor_Dynamic>::iterator pActor2, eCollisionDir& dir )
{
	// The Collision dir passed by reference stands for normal vector from surface of Actor1 that collided

	AABB box1 = pActor1->m_AABBox;
	AABB box2 = pActor2->m_AABBox;

		// TODO: fix if and else if s so that it does EVERY SINGLE CHECK
	// A SEPARATING AXIS WAS FOUND, so exit with no intersection
	if ( box1.m_max.x < box2.m_min.x )
	{
		return false;
	}
	else
	{
		dir = LR;
		return true;
	}

	if ( box1.m_min.x > box2.m_max.x )
	{
		return false;
	}
	else
	{
		dir = RL;
		return true;
	}

	if ( box1.m_max.y < box2.m_min.y )
	{
		return false;
	}
	else
	{
		dir = BT;		
		return true;
	}

	if ( box1.m_min.y > box2.m_max.y )
	{
		return false;
	}
	else
	{
		dir = TB;		
		return true;
	}

	if ( box1.m_max.z < box2.m_min.z )
	{
		return false;
	}
	else
	{
		dir = FB;		
		return true;
	}

	if ( box1.m_min.z > box2.m_max.z )
	{
		return false;
	}
	else
	{
		dir = BF;		
		return true;
	}
}

// True if overlaps, false if no overlap
//bool Scene_Physics::AABBvsAABB( Actor_Dynamic* pActor1, Actor_Dynamic* pActor2 )
//{
// 	AABB box1 = pActor1->m_AABBox;
//	AABB box2 = pActor2->m_AABBox;
//
//	// Exit with no intersection if found separated along an axis
//	if ( box1.m_max.x < box2.m_min.x || box1.m_min.x > box2.m_max.x ) return false;
//	if ( box1.m_max.y < box2.m_min.y || box1.m_min.y > box2.m_max.y ) return false;
//	if ( box1.m_max.z < box2.m_min.z || box1.m_min.z > box2.m_max.z ) return false;
// 
//  // No separating axis found, therefore there is at least one overlapping axis
//	return true;
//}

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
		m_spotLights[ i ].SetLookat( m_collidables[0].GetPosition() );
	}
	for ( int i = m_lightsPerBall; i < m_numLights; i++ )
	{
		m_spotLights[ i ].SetLookat( m_collidables[1].GetPosition() );
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