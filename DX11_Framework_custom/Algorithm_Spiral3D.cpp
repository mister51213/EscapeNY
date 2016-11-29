#include "Algorithm_Spiral3D.h"
#include "MathUtils.h"


using namespace std;

Algorithm_Spiral3D::Algorithm_Spiral3D( ISubGame * pGame )
	:
	Algorithm( pGame )
{
}

// Need to find a way to make this usable by any actor, not just Actor_NPC

// Could just return a vector of Actor parent, then use the returned vector
// to initialize a Vector of Actor_NPCs BEFORE you add the addresses to 
// the draw list.  OR, stop using this function to create actors, but instead
// use them for what they really are, algorithms for making patters.  All we
// really need to return would be a vector of ModelSpecs_W or ModelSpecs_L
// structs, then in the sub game, construct the actors there using the return 
// vector.
vector<Actor_NPC> Algorithm_Spiral3D::MakePatternNPC( int numActors )
{
	vector<Actor_NPC> actorsSUB;
	actorsSUB.reserve( numActors );
	ModelSpecs_W specs = { { 1.f, -30.f, 1.f },{ 0.f,0.f,0.f },{ 1.2f,0.5f,1.3f } };

	float spacing = 1080.f / numActors;

	float x = 1;
	float y = -50;
	float z = 1;

	float increment = 100.f;

	int tex = 0;
	for( int i = 0; i < numActors; i++ )
	{
		float angleStep = static_cast<float>( i )*spacing;

		x = sin( angleStep*g_radian ) * 30;
		z = cos( angleStep*g_radian ) * 30;
		y += .5;

		specs.position.x = x;
		specs.position.y = y;
		specs.position.z = z;

		actorsSUB.push_back( Actor_NPC( specs, (eTexture)tex, ModelSpecs_L() ) );

		x = sin( angleStep*g_radian ) * increment;
		z = cos( angleStep*g_radian ) * increment;
		increment -= 20.f;

		if( tex<4 )
		{
			tex++;
		}
		else if( tex == 4 )
		{
			tex = 0;
		}
	}

	return actorsSUB;
}

//vector<Actor> Algorithm_Spiral3D::MakePattern( int numActors )
//{
//	vector<Actor> actorsSUB;
//	actorsSUB.reserve( numActors );
//	ModelSpecs_W specs{};
//	specs.scale = { 1.2f,0.5f,1.3f };
//
//	const float numSpirals = 3.f;
//	float recipActorCount = 1.0f / static_cast<float>( numActors );
//	float spacing = (360.f * numSpirals) * recipActorCount;
//	
//	const float baseYPosition = -50.f;
//	float increment = 100.f;
//
//	int tex = 0;
//	for( int i = 0; i < numActors; i++ )
//	{
//		float angleStep = static_cast<float>( i )*spacing;
//
//		const float x = sin( angleStep*radian ) * 30.f;
//		const float z = cos( angleStep*radian ) * 30.f;
//		const float y = baseYPosition + (static_cast<float>(i) * .5f);
//
//		specs.position = { x,y,z };
//		++tex;
//		tex %= 5;
//		actorsSUB.push_back( 
//			Actor( specs, static_cast<eTexture>( tex ), ModelSpecs_L() ) 
//		);
//	}
//	return actorsSUB;
//}
