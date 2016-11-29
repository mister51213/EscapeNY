#include "Algorithm_Grid3D.h"
#include "MathUtils.h"

using namespace std;
using namespace DirectX;

///////////////////////////////////////
// Old inherited function

//vector<Actor> Algorithm_Grid3D::MakePattern( int numActors )
//{
//	numActors -= ( numActors % 25 );
//
//	const int m_numColumns = numActors / 25;
//	const int m_numRows = numActors / 25;
//	const int m_numZ = numActors / 25;
//
//	vector<Actor> actorsSUB;
//	actorsSUB.reserve( numActors );
//
//	ModelSpecs_W specs = {};
//	specs.scale = { .01f, .01f, .01f };
//	const float spacing = 15.f;
//	for( float i = 0; i < m_numColumns; i++ )
//	{
//		specs.position.x = i * spacing;
//		for( float j = 0; j < m_numRows; j++ )
//		{
//			specs.position.y = j * spacing;
//			for( float k = 0; k < m_numZ; k++ )
//			{
//				specs.position.z = k * spacing;			
//				actorsSUB.push_back( Actor( specs, AsphaltFresh, ModelSpecs_L() ) );
//			}
//		}
//	}
//	return actorsSUB;
//}

///////////////////////////////////////

Algorithm_Grid3D::Algorithm_Grid3D()
	:
	Algorithm( nullptr )
{
}

//TEST//

vector<Actor> Algorithm_Grid3D::MakePattern( int )
{
	return vector<Actor>();
}

vector<Actor_NPC> Algorithm_Grid3D::MakePatternNPC( int numActors, XMFLOAT3 worldOffset )
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


	ModelSpecs_W specs = { { 0.f,0.f,0.f }, { 0.f,0.f,0.f },{ .3f,.4f,.5f } };
	for( float i = 0; i < m_numColumns; i++ )
	{
		for( float j = 0; j < m_numRows; j++ )
		{
			for( float k = 0; k < m_numZ; k++ )
			{
				specs.position.x = i*15.f + worldOffset.x;
				specs.position.y = j*15.f + worldOffset.y;
				specs.position.z = k*15.f + worldOffset.z;
				specs.scale.x += .01f;
				specs.scale.y += .015f;
				specs.scale.z += .02f;

				int index = actorsSUB.size();
				actorsSUB.push_back( Actor_NPC(/*m_pInput, */specs, SharkSkin, ModelSpecs_L() ) );
			}
		}
	}

	return actorsSUB;
}