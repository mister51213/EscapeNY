#include "Algorithm_Grid3D.h"

Algorithm_Grid3D::Algorithm_Grid3D()
	:
	Algorithm( nullptr )
{
}

vector<Actor_NPC> Algorithm_Grid3D::MakePatternNPC( int numActors )
{
	int m_numColumns;
	int m_numRows;
	int m_numZ;

	if( numActors != 0 && numActors % 25 == 0 )
	{
		m_numColumns = numActors / 25;
		m_numRows = numActors / 25;
		m_numZ = numActors / 25;
	}
	else
	{
		numActors -= ( numActors % 25 );
		m_numColumns = numActors / 25;
		m_numRows = numActors / 25;
		m_numZ = numActors / 25;
	}
	vector<Actor_NPC> actorsSUB;
	actorsSUB.reserve( numActors );

	ModelSpecs_W specs = { { 0.f, 0.f, 0.f },{ 0.f,0.f,0.f },{ .3f,.4f,.5f } };
	for( float i = 0; i < m_numColumns; i++ )
	{
		for( float j = 0; j < m_numRows; j++ )
		{
			for( float k = 0; k < m_numZ; k++ )
			{
				specs.position.x = i*15.f;
				specs.position.y = j*15.f;
				specs.position.z = k*15.f;
				specs.scale.x += .01f;
				specs.scale.y += .015f;
				specs.scale.z += .02f;

				int index = actorsSUB.size();
				actorsSUB.push_back( Actor_NPC(specs, Underwater6, ModelSpecs_L() ) );
			}
		}
	}

	return actorsSUB;
}