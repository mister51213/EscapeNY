#include "Algorithm_Grid3D.h"

vector<Actor> Algorithm_Grid3D::MakePattern( int numActors )
{
	numActors -= ( numActors % 25 );

	const int m_numColumns = numActors / 25;
	const int m_numRows = numActors / 25;
	const int m_numZ = numActors / 25;

	vector<Actor> actorsSUB;
	actorsSUB.reserve( numActors );

	ModelSpecs_W specs = {};
	specs.scale = { .01f, .01f, .01f };
	const float spacing = 15.f;
	for( float i = 0; i < m_numColumns; i++ )
	{
		specs.position.x = i * spacing;
		for( float j = 0; j < m_numRows; j++ )
		{
			specs.position.y = j * spacing;
			for( float k = 0; k < m_numZ; k++ )
			{
				specs.position.z = k * spacing;			
				actorsSUB.push_back( Actor( specs, AsphaltFresh, ModelSpecs_L() ) );
			}
		}
	}
	return actorsSUB;
}
