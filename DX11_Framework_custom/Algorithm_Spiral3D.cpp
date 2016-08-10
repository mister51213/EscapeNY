#include "Algorithm_Spiral3D.h"

Algorithm_Spiral3D::Algorithm_Spiral3D( ISubGame * pGame )
	:
	Algorithm( pGame )
{
}

vector<Actor> Algorithm_Spiral3D::MakePattern( int numActors )
{
	vector<Actor> actorsSUB;
	actorsSUB.reserve( numActors );
	ModelSpecs_W specs{};
	specs.scale = { 1.2f,0.5f,1.3f };

	const float numSpirals = 3.f;
	float recipActorCount = 1.0f / static_cast<float>( numActors );
	float spacing = (360.f * numSpirals) * recipActorCount;
	
	const float baseYPosition = -50.f;
	float increment = 100.f;

	int tex = 0;
	for( int i = 0; i < numActors; i++ )
	{
		float angleStep = static_cast<float>( i )*spacing;

		const float x = sin( angleStep*radian ) * 30.f;
		const float z = cos( angleStep*radian ) * 30.f;
		const float y = baseYPosition + (static_cast<float>(i) * .5f);

		specs.position = { x,y,z };
		++tex;
		tex %= 5;
		actorsSUB.push_back( 
			Actor( specs, static_cast<eTexture>( tex ), ModelSpecs_L() ) 
		);
	}
	return actorsSUB;
}
