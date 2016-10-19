/***********************************************************************************
File: PrimitiveFactory.cpp
Function: Create basic 3d primitives using vector math. Can be combined with
texture functionality for more detailed graphics.

***************************************************************************************/
#include "PrimitiveFactory.h"
#include "Utilities.h"

using namespace DirectX;

void PrimitiveFactory::CreateTriangle( const ModelSpecs_L &Specs )
{
	auto CreateVertexList = [ &Specs ]()
	{
		auto extentHalf = Specs.size * 0.5f;

		return std::vector<XMFLOAT3>
		{
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z },
			{ Specs.center.x, Specs.center.y + extentHalf.y, Specs.center.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z }
		};
	};
	auto CreateNormalsList = []()
	{
		return std::vector<DirectX::XMFLOAT3>
		{
			{ 0.f, 0.f, -1.f },
			{ 0.f, 0.f, -1.f },
			{ 0.f, 0.f, -1.f }
		};
	};
	auto CreateUVList = []()
	{
		return std::vector<DirectX::XMFLOAT2>
		{
			{ 0.f, 1.f },
			{ 0.5f, 0.f },
			{ 1.f, 1.f }
		};
	};

	PrimitiveFactory::ClearAllBuffers();

	PrimitiveFactory::vertices = CreateVertexList();
	PrimitiveFactory::normals = CreateNormalsList();
	PrimitiveFactory::uvs = CreateUVList();

	Common( Specs );
}

void PrimitiveFactory::CreatePlane( const ModelSpecs_L &Specs )
{
	auto CreateVertexList = [ &Specs ]()
	{
		// Extent half is size of the object in each direction
		auto extentHalf = Specs.size * .5f;

		return std::vector<DirectX::XMFLOAT3>
		{
			{ Specs.center.x - extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z }
		};

	};
	auto CreateNormalsList = []()
	{
		return std::vector<DirectX::XMFLOAT3>
		{
			{ 0.f, 0.f, -1.f },
			{ 0.f, 0.f, -1.f },
			{ 0.f, 0.f, -1.f },
			{ 0.f, 0.f, -1.f },
			{ 0.f, 0.f, -1.f },
			{ 0.f, 0.f, -1.f }
		};
	};
	auto CreateUVList = []()
	{
		return std::vector<DirectX::XMFLOAT2>
		{
			{ 0.f, 0.f },
			{ 1.f, 0.f },
			{ 0.f, 1.f },
			{ 0.f, 1.f },
			{ 1.f, 0.f },
			{ 1.f, 1.f }
		};
	};

	PrimitiveFactory::ClearAllBuffers();

	PrimitiveFactory::vertices = CreateVertexList();
	PrimitiveFactory::normals = CreateNormalsList();
	PrimitiveFactory::uvs = CreateUVList();

	Common( Specs );
}

void PrimitiveFactory::CreatePlaneNM( const ModelSpecs_L & Specs )
{
	CreatePlane( Specs );

	auto vertCount = vertices.size();
	tangents.resize( vertCount );
	binormals.resize( vertCount );
	normals.resize( vertCount );

	for ( int i = 0; i < vertCount; i += 3 )
	{
		auto edge10 = vertices[ i + 1 ] - vertices[ i ];
		auto edge20 = vertices[ i + 2 ] - vertices[ i ];

		auto tEdge10 = uvs[ i + 1 ] - uvs[ i ];
		auto tEdge20 = uvs[ i + 2 ] - uvs[ i ];

		tangents[ i ] = CalculateTangent( edge10, tEdge10 );
		binormals[ i ] = CalculateBiNormal( tangents[ i ], normals[ i ] );

		tangents[ i + 2 ] = tangents[ i + 1 ] = tangents[ i ];
		binormals[ i + 2 ] = binormals[ i + 1 ] = binormals[ i ];
		normals[ i + 2 ] = normals[ i + 1 ] = normals[ i ];
	}
}

void PrimitiveFactory::CreateCube( const ModelSpecs_L &Specs )
{
	auto CreateVertexList = [ &Specs ]()->std::vector<DirectX::XMFLOAT3>
	{
		auto extentHalf = Specs.size * .5f;

		return std::vector<DirectX::XMFLOAT3>
		{
			// Left
			{ Specs.center.x - extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z - extentHalf.z },

				// Right
			{ Specs.center.x + extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z + extentHalf.z },

				// Bottom
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z + extentHalf.z },

				// Top
			{ Specs.center.x - extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z - extentHalf.z },

				// Front
			{ Specs.center.x - extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z - extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z - extentHalf.z },

				// Back
			{ Specs.center.x + extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y + extentHalf.y, Specs.center.z + extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y, Specs.center.z + extentHalf.z }
		};
	};
	auto CreateNormalsList = []()->std::vector<DirectX::XMFLOAT3>
	{
		return std::vector<DirectX::XMFLOAT3>
		{
			// Left
			{ -1.f, 0.f, 0.f },
			{ -1.f, 0.f, 0.f },
			{ -1.f, 0.f, 0.f },
			{ -1.f, 0.f, 0.f },
			{ -1.f, 0.f, 0.f },
			{ -1.f, 0.f, 0.f },

				// Right
			{ 1.f, 0.f, 0.f },
			{ 1.f, 0.f, 0.f },
			{ 1.f, 0.f, 0.f },
			{ 1.f, 0.f, 0.f },
			{ 1.f, 0.f, 0.f },
			{ 1.f, 0.f, 0.f },

				// Bottom
			{ 0.f, -1.f, 0.f },
			{ 0.f, -1.f, 0.f },
			{ 0.f, -1.f, 0.f },
			{ 0.f, -1.f, 0.f },
			{ 0.f, -1.f, 0.f },
			{ 0.f, -1.f, 0.f },

				// Top
			{ 0.f, 1.f, 0.f },
			{ 0.f, 1.f, 0.f },
			{ 0.f, 1.f, 0.f },
			{ 0.f, 1.f, 0.f },
			{ 0.f, 1.f, 0.f },
			{ 0.f, 1.f, 0.f },

				// Front
			{ 0.f, 0.f, -1.f },
			{ 0.f, 0.f, -1.f },
			{ 0.f, 0.f, -1.f },
			{ 0.f, 0.f, -1.f },
			{ 0.f, 0.f, -1.f },
			{ 0.f, 0.f, -1.f },

				// Back
			{ 0.f, 0.f, 1.f },
			{ 0.f, 0.f, 1.f },
			{ 0.f, 0.f, 1.f },
			{ 0.f, 0.f, 1.f },
			{ 0.f, 0.f, 1.f },
			{ 0.f, 0.f, 1.f }
		};
	};
	auto CreateUVList = []()
	{
		return std::vector<XMFLOAT2>
		{
			// Left
			{ 0.f, 0.f },
			{ 1.f, 0.f },
			{ 0.f, 1.f },
			{ 0.f, 1.f },
			{ 1.f, 0.f },
			{ 1.f, 1.f },
				// Right
			{ 0.f, 0.f },
			{ 1.f, 0.f },
			{ 0.f, 1.f },
			{ 0.f, 1.f },
			{ 1.f, 0.f },
			{ 1.f, 1.f },
				// Bottom
			{ 0.f, 0.f },
			{ 1.f, 0.f },
			{ 0.f, 1.f },
			{ 0.f, 1.f },
			{ 1.f, 0.f },
			{ 1.f, 1.f },
				// Top
			{ 0.f, 0.f },
			{ 1.f, 0.f },
			{ 0.f, 1.f },
			{ 0.f, 1.f },
			{ 1.f, 0.f },
			{ 1.f, 1.f },
				// Front
			{ 0.f, 0.f },
			{ 1.f, 0.f },
			{ 0.f, 1.f },
			{ 0.f, 1.f },
			{ 1.f, 0.f },
			{ 1.f, 1.f },
				// Back
			{ 0.f, 0.f },
			{ 1.f, 0.f },
			{ 0.f, 1.f },
			{ 0.f, 1.f },
			{ 1.f, 0.f },
			{ 1.f, 1.f }
		};
	};

	PrimitiveFactory::ClearAllBuffers();

	PrimitiveFactory::vertices = CreateVertexList();
	PrimitiveFactory::normals = CreateNormalsList();
	PrimitiveFactory::uvs = CreateUVList();

	Common( Specs );
}

void PrimitiveFactory::CreateSphereNM( const ModelSpecs_L &Specs, const float radiusGlobe, int vertices )
{
	// sin values for theta = 90, 75, 60, 45, 30, 15, 0
	const float sinValues[ 7 ] =
	{
		1.0f,
		0.9659258f,
		0.8660254f,
		0.707106f,
		0.5f,
		0.258819f,
		0.0f
	};

	// cos values for theta = 90, 75, 60, 45, 30, 15, 0
	const float cosValues[ 7 ] =
	{
		0.0f,
		0.258819f,
		0.5f,
		0.707106f,
		0.8660254f,
		0.9659258f,
		1.0f
	};

	// TODO: optimize by adding array of const pre-multiplied squares of cos and sin values

	// TODO: OFFSET ALL OF THESE POINTS BY LOCAL SPECS VALUES

	// Scale main globe radius by average of local specs
	const float gRadius = radiusGlobe*Magnitude( Specs.size );

	// Master and quadrants
	//vector<DirectX::XMFLOAT3> quadrant1( 37 );
	vector<DirectX::XMFLOAT3> masterSphere;
	//	masterSphere.resize( quadrant1.size() * 8 + 2 );

	/////////////// ADD VERTEX AT FAR POLE OF SPHERE ////////
	masterSphere.push_back( { 0.0f, 0.0f, gRadius } );

	vector<int> intermediateIndices;

	/////////////////// Make one positive quadrant ///////////////////
	//// loop through cos values along z axis (from far pole of globe to its center)
	//for ( int z = 5; z >= 0; z-- ) // NOTE: end at 6, because we don't need the single point at the pole.
	//{
	//	// radius of each slice = sin of angle PHI at that slice
	//	float sliceRadius = sinValues[ z ] * gRadius; // scale down radius by sin of the slice
	//	float slicePosZ = cosValues[ z ] * gRadius;
	//	// TODO: Each quadrant should only interate through 1ST 6 NUMBERS,
	//	// because the starting vertex of next quadrant = ending vert of previous
	//	for ( int i = 6; i >= 0; i-- )
	//	{
	//		int index = z * 6 + i;
	//		quadrant1[ index ] = 
	//		{ 
	//			sliceRadius*cosValues[ i ], 
	//			sliceRadius*sinValues[ i ], 
	//			slicePosZ 
	//		};			
	//		// FIRST SLICE Q1
	//		intermediateIndices.push_back(index);
	//		// FIRST SLICE Q2
	//		intermediateIndices.push_back(index+37);
	//		// FIRST SLICE Q3
	//		intermediateIndices.push_back(index+37*2);
	//		// FIRST SLICE Q4
	//		intermediateIndices.push_back(index+37*3);
	//	}
	//}

	///////////////// Make one positive quadrant ///////////////////
	vector<vector<DirectX::XMFLOAT3>> quadrant1;
	quadrant1.resize( 6 );
	for(auto& latitude: quadrant1 )
	{
		latitude.resize( 6 );
	}

	// loop through cos values along z axis (from far pole of globe to its center)
	for ( int z = 5; z >= 0; z-- ) // NOTE: We don't need the single point at the pole.
	{
		// radius of each slice = sin of angle PHI at that slice
		float sliceRadius = sinValues[ z ] * gRadius; // scale down radius by sin of the slice
		float slicePosZ = cosValues[ z ] * gRadius;

		// TODO: Each quadrant should only interate through 1ST 6 NUMBERS,
		// because the starting vertex of next quadrant = ending vert of previous

		for ( int i = 5; i >= 0; i-- )
		{
			quadrant1[z][i] = {sliceRadius*cosValues[ i ], sliceRadius*sinValues[ i ], slicePosZ };			

			// COMPLETE THE CIRCLE
			int index = 6 * z + i;
			intermediateIndices.push_back(index);
			intermediateIndices.push_back(index+37);
			intermediateIndices.push_back(index+37*2);
			intermediateIndices.push_back(index+37*3);

		}
	}

	////////////////////////////////////////////////////////////
	//////////////// REFLECT TO MAKE OTHER 7 QUADRANTS /////////
	////////////////////////////////////////////////////////////
	//int quadSize = quadrant1.size();

	//////////////////// FAR TOP RIGHT //////////////////////////////
	//for ( int i = 0; i < quadSize; i++ )
	//{
	//	masterSphere.push_back( quadrant1[ i ] );
	//}
	//////////////////// FAR TOP LEFT //////////////////////////////

	//for ( int i = 0; i < quadSize; i++ )
	//{
	//	masterSphere.push_back( { -quadrant1[ i ].x, quadrant1[ i ].y, quadrant1[ i ].z, } );
	//}
	//////////////////// FAR BOTTOM LEFT //////////////////////////////
	//for ( int i = 0; i < quadSize; i++ )
	//{
	//	masterSphere.push_back( { -quadrant1[ i ].x, -quadrant1[ i ].y, quadrant1[ i ].z, } );
	//}
	//////////////////// FAR BOTTOM RIGHT //////////////////////////////
	//for ( int i = 0; i < quadSize; i++ )
	//{
	//	masterSphere.push_back( { quadrant1[ i ].x, -quadrant1[ i ].y, quadrant1[ i ].z, } );
	//}

	//////////////////// NEAR TOP RIGHT //////////////////////////////
	//for ( int i = 0; i < quadSize; i++ )
	//{
	//	masterSphere.push_back( { quadrant1[ i ].x, quadrant1[ i ].y, -quadrant1[ i ].z, } );
	//}
	//////////////////// NEAR TOP LEFT //////////////////////////////
	//for ( int i = 0; i < quadSize; i++ )
	//{
	//	masterSphere.push_back( { -quadrant1[ i ].x, quadrant1[ i ].y, -quadrant1[ i ].z, } );
	//}
	//////////////////// NEAR BOTTOM LEFT //////////////////////////////
	//for ( int i = 0; i < quadSize; i++ )
	//{
	//	masterSphere.push_back( { -quadrant1[ i ].x, -quadrant1[ i ].y, -quadrant1[ i ].z, } );
	//}
	//////////////////// NEAR BOTTOM RIGHT //////////////////////////////
	//for ( int i = 0; i < quadSize; i++ )
	//{
	//	masterSphere.push_back( { quadrant1[ i ].x, -quadrant1[ i ].y, -quadrant1[ i ].z, } );
	//}


	/////////////// END W VERTEX AT OPPOSITE POLE ///////////////////
	//masterSphere[masterSize-1]={ 0.0f,0.0f,-gRadius };
	masterSphere.push_back( { 0.0f, 0.0f, -gRadius } );

	//////////////// MAKE INDICES /////////////////////////
	// Reorder the vertices into index triplets corresponding to triangles
	// for DirectX pipeline to draw

	vector<DWORD> sphereIndices;

	// TODO: group indices into continuous loops first
	//////////// DRAW TOP /////////////////
	for( int i = 0; i < 24; i++ )
	{
		// All 7 triangles start with index of pole
		sphereIndices.push_back( 0 );
		sphereIndices.push_back( intermediateIndices[i ] );
		sphereIndices.push_back( intermediateIndices[i + 1] );
	}
	// knit together last and first vertices
	sphereIndices.push_back( 0 );
	sphereIndices.push_back( intermediateIndices[27] );
    sphereIndices.push_back( intermediateIndices[0] );

	bool flipFlag = false;
	//////////// DRAW NEXT BELT /////////////////
	for( int i = 0; i < 24; i++ )
	{
		if( !flipFlag )
		{	
			sphereIndices.push_back( intermediateIndices[ i ] );
			sphereIndices.push_back( intermediateIndices[ i + 24 ] );
			sphereIndices.push_back( intermediateIndices[ i + 1 ] );
			flipFlag = true;
		}
		else
		{
			sphereIndices.push_back( intermediateIndices[ i + 1 ] );
			sphereIndices.push_back( intermediateIndices[ i + 24 ] );
			sphereIndices.push_back( intermediateIndices[ i + 25 ] );
			flipFlag = false;
		}
	}

	// TODO: make outer for loop, iterating thru number of slices
	// TODO: multiply by numslices
		//////////// DRAW NEXT BELT /////////////////
	int numslices=3;
	for( int i = 0; i < 24; i++ )
	{
		if( !flipFlag )
		{	
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices-1] );
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices ] );
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices-1] );
			flipFlag = true;
		}
		else
		{
			sphereIndices.push_back( intermediateIndices[ i + 1 ] );
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices ] );
			sphereIndices.push_back( intermediateIndices[ i + 25*numslices ] );
			flipFlag = false;
		}
	}

		//////////// DRAW NEXT BELT /////////////////
	int numslices1=4;
	for( int i = 0; i < 24; i++ )
	{
		if( !flipFlag )
		{	
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices1-1] );
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices1 ] );
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices1-1] );
			flipFlag = true;
		}
		else
		{
			sphereIndices.push_back( intermediateIndices[ i + 1 ] );
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices1 ] );
			sphereIndices.push_back( intermediateIndices[ i + 25*numslices1 ] );
			flipFlag = false;
		}
	}

		//////////// DRAW NEXT BELT /////////////////
	int numslices2=5;
	for( int i = 0; i < 24; i++ )
	{
		if( !flipFlag )
		{	
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices1-1] );
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices1 ] );
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices1-1] );
			flipFlag = true;
		}
		else
		{
			sphereIndices.push_back( intermediateIndices[ i + 1 ] );
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices1 ] );
			sphereIndices.push_back( intermediateIndices[ i + 25*numslices1 ] );
			flipFlag = false;
		}
	}

			//////////// DRAW NEXT BELT /////////////////
	int numslices3=6;
	for( int i = 0; i < 24; i++ )
	{
		if( !flipFlag )
		{	
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices1-1] );
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices1 ] );
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices1-1] );
			flipFlag = true;
		}
		else
		{
			sphereIndices.push_back( intermediateIndices[ i + 1 ] );
			sphereIndices.push_back( intermediateIndices[ i + 24*numslices1 ] );
			sphereIndices.push_back( intermediateIndices[ i + 25*numslices1 ] );
			flipFlag = false;
		}
	}



	// TODO: Test to see if sorting helps
	// Sort algorithm is complete, but haven't implemented any triangle 
	// creation.  First everything is sorted by Y (Top to Bottom) then by Z
	// (Front to Back), then by X (Left to Right)
	//Sort( masterSphere );

	PrimitiveFactory::ClearAllBuffers();

	// LOAD VERTICES INTO PRIMITIVE FACTORY LIST //
	int masterSize = masterSphere.size();
	PrimitiveFactory::vertices = masterSphere;

	//////////// TEST //////////////
	PrimitiveFactory::indices = sphereIndices;

	////////////// CREATE UVs ///////////////////////////////////////
	//PrimitiveFactory::uvs.resize( masterSize);

	// UV coordinates are between 0 and 1, except when wrapping textures.
	// The top and left of the texture are the origin and bottom right 
	// are both 1.  To get the vertices in this range we:
	// First normalize them.  This gets us between -1 and 1.  
	// Second we invert the Y value so that the top of the sphere is 
	//		drawn using the top of the texture.
	// Third scale the image by half, giving us a range of 1 (-.5 to .5)
	// Fourth add .5 to X and Y getting us between 0 and 1

	// This means that the sphere will be flattened and the back and front 
	// will show the same image.
	const auto uvOffset = XMFLOAT2( .5f, .5f );
	for ( const auto &vertex : masterSphere )
	{
		const auto nVertex = Normalize( vertex );
		const auto uv = ( XMFLOAT2( nVertex.x, -( nVertex.y ) ) * .5f ) + uvOffset;
		PrimitiveFactory::uvs.push_back( uv );
	}

	////////////// CREATE NORMALs ///////////////////////////////////////
	//PrimitiveFactory::normals.resize( masterSize );

	for ( int i = 0; i < masterSize; i++ )
	{
		XMFLOAT3 radiusVector = masterSphere[ i ] - Specs.center;
		XMFLOAT3 normal = Normalize( radiusVector );
		PrimitiveFactory::normals.push_back( normal );

		//// CREATE BINORMALS & TANGENTS (Just filler) ////
		XMFLOAT3 edge10;
		XMFLOAT3 edge20;
		XMFLOAT2 tEdge10;
		XMFLOAT2 tEdge20;

		if ( i + 3 < masterSize )
		{
			edge10 = masterSphere[ i + 1 ] - masterSphere[ i + 0 ];
			edge20 = masterSphere[ i + 2 ] - masterSphere[ i + 0 ];
			tEdge10 = uvs[ i + 1 ] - uvs[ i + 0 ]; 
			tEdge20 = uvs[ i + 2 ] - uvs[ i + 0 ];
		}
		else // do it in reverse
		{
			edge10 = masterSphere[ i - 0 ] - masterSphere[ i - 1 ];
			edge20 = masterSphere[ i + 0 ] - masterSphere[ i - 2 ];
			tEdge10 = uvs[ i + 0 ] - uvs[ i - 1 ];
			tEdge20 = uvs[ i + 0 ] - uvs[ i - 2 ];
		}

		XMFLOAT3 tangent = CalculateTangent( edge10, tEdge10 );
		XMFLOAT3 binormal = CalculateBiNormal( tangent, normal );

		PrimitiveFactory::binormals.push_back( tangent );
		PrimitiveFactory::tangents.push_back( binormal );
	}

	//Common( Specs );
}

void PrimitiveFactory::CreateCubeNM( const ModelSpecs_L & Specs )
{
	CreateCube( Specs );

	int vertCount = vertices.size();
	tangents.resize( vertCount );
	binormals.resize( vertCount );
	normals.resize( vertCount );

	// Create the tangent matrix for each face (set of three vertex positions)
	for ( int i = 0; i < vertCount; i += 3 )
	{
		auto edge10 = vertices[ i + 1 ] - vertices[ i + 0 ];
		auto edge20 = vertices[ i + 2 ] - vertices[ i + 0 ];

		auto tEdge10 = uvs[ i + 1 ] - uvs[ i + 0 ];
		auto tEdge20 = uvs[ i + 2 ] - uvs[ i + 0 ];

		tangents[ i ] = CalculateTangent( edge10, tEdge10 );
		normals[ i ] = CalculateNormal( edge10, edge20 );
		binormals[ i ] = CalculateBiNormal( tangents[ i ], normals[ i ] );

		tangents[ i + 2 ] = tangents[ i + 1 ] = tangents[ i ];
		binormals[ i + 2 ] = binormals[ i + 1 ] = binormals[ i ];
		normals[ i + 2 ] = normals[ i + 1 ] = normals[ i ];
	}
}

void PrimitiveFactory::CreateMeshNM( const wstring & Filename )
{
	///////////////////////////
	// LOAD MODEL FROM FILE  //
	///////////////////////////
	int vertexCount, indexCount;

	auto VertsFromF = [ &Filename, &vertexCount, &indexCount ]()->bool
	{
		ifstream file;
		char input;
		int i;

		// Open the model file.
		file.open( Filename );

		// If it could not open the file then exit.
		if ( file.fail() )
		{
			return false;
		}

		// Read up to the value of vertex count.
		file.get( input );
		while ( input != ':' )
		{
			file.get( input );
		}

		// Read in the vertex count.
		file >> vertexCount;

		// Set the number of indices to be the same as the vertex count.
		indexCount = vertexCount;

		// Read up to the beginning of the data.
		file.get( input );
		while ( input != ':' )
		{
			file.get( input );
		}
		file.get( input );
		file.get( input );

		// load straight into static members
		vertices.resize( vertexCount );
		uvs.resize( vertexCount );
		normals.resize( vertexCount );

		for ( i = 0; i < vertexCount; i++ )
		{
			file >> vertices[ i ].x >> vertices[ i ].y >> vertices[ i ].z;
			file >> uvs[ i ].x >> uvs[ i ].y;
			file >> normals[ i ].x >> normals[ i ].y >> normals[ i ].z;
		}

		// Close the model file.
		file.close();

		return true;
	};

	auto VertsFromB = [ &Filename, &vertexCount, &indexCount ]()->bool
	{
		ifstream file( Filename, std::ios::binary ); // open file in binary mode
		if ( !file.is_open() )
		{
			return false;
		}

		vertexCount = 0; // read first 32 bits into vertexCount
		file.read( reinterpret_cast<char*>( &vertexCount ), sizeof( int ) );

		// Pack into member variables of PrimFactory
		vertices.resize( vertexCount );
		uvs.resize( vertexCount );
		normals.resize( vertexCount );
		indices.resize( vertexCount );

		auto expectedSize = vertexCount * sizeof( VertexBufferTypePosUvNormal );
		auto altSize = vertexCount * sizeof( VertexBufferTypeAllInOne );
		auto curFilePosition = file.tellg();
		file.seekg( 0, std::ios::end );
		auto fileSize = file.tellg().seekpos();
		file.seekg( curFilePosition, std::ios::beg );

		fileSize -= curFilePosition;
		if ( fileSize == altSize )
		{
			vector<VertexBufferTypeAllInOne> vertList( vertexCount );
			file.read( reinterpret_cast<char*>( vertList.data() ), sizeof( VertexBufferTypeAllInOne )*vertexCount );

			for ( int i = 0; i < vertexCount; i++ )
			{
				vertices[ i ] = vertList[ i ].position;
				uvs[ i ] = vertList[ i ].uv;
				normals[ i ] = vertList[ i ].normal;
				indices[ i ] = i; // TODO: not sure about this
			}
		}
		else
		{
			vector<VertexBufferTypePosUvNormal> vertList( vertexCount );
			file.read( reinterpret_cast<char*>( vertList.data() ), sizeof( VertexBufferTypePosUvNormal )*vertexCount );

			for ( int i = 0; i < vertexCount; i++ )
			{
				vertices[ i ] = vertList[ i ].position;
				uvs[ i ] = vertList[ i ].uv;
				normals[ i ] = vertList[ i ].normal;
				indices[ i ] = i; // TODO: not sure about this
			}
		}

		file.close();
		return true;
	};

	PrimitiveFactory::ClearAllBuffers();

	// TODO: Check GameView lines 60~82 for exact data it needs to load a model.
	// store file data into ModelType pModel list of verts,uvs,norms
	//bool result = VertsFromF();
	bool result = VertsFromB();

	///////////////////////////////////////  
	// LOAD MODEL DATA into vertex array //
	///////////////////////////////////////
	indexCount = vertexCount;
	indices.resize( indexCount );

	tangents.resize( vertexCount );
	binormals.resize( vertexCount );

	for ( int i = 0; i < vertexCount; i += 3 )
	{
		auto edge10 = vertices[ i + 1 ] - vertices[ i ];
		auto edge20 = vertices[ i + 2 ] - vertices[ i ];
		auto tEdge10 = uvs[ i + 1 ] - uvs[ i ];
		auto tEdge20 = uvs[ i + 2 ] - uvs[ i ];

		// Calculate the denominator of the tangent/binormal equation.
		float den = 1.0f / (
			( tEdge10.x * tEdge20.y ) - ( tEdge10.y * tEdge20.x )
			);

		// Calculate the cross products and multiply by the coefficient to get 
		// the tangent and binormal.
		XMFLOAT3 tangent{}, binormal{};
		tangent.x = ( tEdge20.y * edge10.x - tEdge10.y * edge20.x ) * den;
		tangent.y = ( tEdge20.y * edge10.y - tEdge10.y * edge20.y ) * den;
		tangent.z = ( tEdge20.y * edge10.z - tEdge10.y * edge20.z ) * den;

		binormal.x = ( tEdge10.x * edge20.x - tEdge20.x * edge10.x ) * den;
		binormal.y = ( tEdge10.x * edge20.y - tEdge20.x * edge10.y ) * den;
		binormal.z = ( tEdge10.x * edge20.z - tEdge20.x * edge10.z ) * den;

		tangent = Normalize( tangent );
		binormal = Normalize( binormal );

		tangents[ i + 2 ] = tangents[ i + 1 ] = tangents[ i ] = tangent;
		binormals[ i + 2 ] = binormals[ i + 1 ] = binormals[ i ] = binormal;
	}

}

// Custom file format - Each line contains position vector(x, y, z), texture coordinates(tu, tv), and normal vector(nx, ny, nz). 
void PrimitiveFactory::CreateMesh(
	const wstring& fileName )
{
	////////////////////////////////  
	// LOAD MODEL FROM FILE
	////////////////////////////////
	ID3D11Buffer *pVertexBuffer, *pIndexBuffer;
	int vertexCount, indexCount;
	ModelType* pModel;

	auto VertsFromF = [ &fileName, &vertexCount, &indexCount, &pModel ]()->bool
	{
		ifstream fin;
		char input;
		int i;

		// Open the model file.
		fin.open( fileName );

		// If it could not open the file then exit.
		if ( fin.fail() )
		{
			return false;
		}

		// Read up to the value of vertex count.
		fin.get( input );
		while ( input != ':' )
		{
			fin.get( input );
		}

		// Read in the vertex count.
		fin >> vertexCount;

		// Set the number of indices to be the same as the vertex count.
		indexCount = vertexCount;

		// Create the model using the vertex count that was read in.
		pModel = new ModelType[ vertexCount ];
		if ( !pModel )
		{
			return false;
		}

		// Read up to the beginning of the data.
		fin.get( input );
		while ( input != ':' )
		{
			fin.get( input );
		}
		fin.get( input );
		fin.get( input );

		// load straight into static members
		vertices.resize( vertexCount );
		uvs.resize( vertexCount );
		normals.resize( vertexCount );

		for ( i = 0; i < vertexCount; i++ )
		{
			fin >> vertices[ i ].x >> vertices[ i ].y >> vertices[ i ].z;
			fin >> uvs[ i ].x >> uvs[ i ].y;
			fin >> normals[ i ].x >> normals[ i ].y >> normals[ i ].z;
		}

		// Close the model file.
		fin.close();

		return true;
	};

	auto VertsFromB = [ &fileName, &vertexCount, &indexCount, &pModel ]()->bool
	{
		ifstream file( fileName, std::ios::binary ); // open file in binary mode

		if ( file.fail() )
		{
			return false;
		}

		vertexCount = 0; // read first 32 bits into vertexCount
		file.read( reinterpret_cast<char*>( &vertexCount ), sizeof( int ) );

		// pack into vector of structs
		// TODO: change OBJ to BINARY LOADER
		vector<VertexBufferTypeAllInOne> vertList( vertexCount );
		file.read( reinterpret_cast<char*>( vertList.data() ), sizeof( VertexBufferTypeAllInOne )*vertexCount );


		// Pack into member variables of PrimFactory
		vertices.resize( vertexCount );
		uvs.resize( vertexCount );
		normals.resize( vertexCount );
		indices.resize( vertexCount );

		for ( int i = 0; i < vertexCount; i++ )
		{
			vertices[ i ] = vertList[ i ].position;
			uvs[ i ] = vertList[ i ].uv;
			normals[ i ] = vertList[ i ].normal;
			indices[ i ] = i; // TODO: not sure about this
		}

		file.close();
		return true;
	};

	PrimitiveFactory::ClearAllBuffers();

	// TODO: Check GameView lines 60~82 for exact data it needs to load a model.
	// store file data into ModelType pModel list of verts,uvs,norms
	//bool result = VertsFromF();
	bool result = VertsFromB();

	///////////////////////////////////////  
	// LOAD MODEL DATA into vertex array //
	///////////////////////////////////////
	indexCount = vertexCount;
	indices.resize( indexCount );

	for ( int i = 0; i < indexCount; i++ )
	{
		indices[ i ] = i;
	}
}

void PrimitiveFactory::CreateColor( float R, float G, float B, float A )
{
	color = DirectX::XMFLOAT4( R, G, B, A );
}

std::vector<DirectX::XMFLOAT3> PrimitiveFactory::GetVertices()
{
	return PrimitiveFactory::vertices;
}

std::vector<DirectX::XMFLOAT3> PrimitiveFactory::GetNormals()
{
	return PrimitiveFactory::normals;
}

std::vector<DirectX::XMFLOAT2> PrimitiveFactory::GetUVs()
{
	return PrimitiveFactory::uvs;
}

std::vector<DirectX::XMFLOAT3> PrimitiveFactory::GetTangent()
{
	return tangents;
}

std::vector<DirectX::XMFLOAT3> PrimitiveFactory::GetBiTangent()
{
	return binormals;
}

std::vector<DWORD> PrimitiveFactory::GetIndices()
{
	return PrimitiveFactory::indices;
}

DirectX::XMFLOAT4 PrimitiveFactory::GetColor()
{
	return PrimitiveFactory::color;
}

XMFLOAT3 PrimitiveFactory::CalculateTangent(
	const XMFLOAT3 &Edge10,
	const XMFLOAT2& tEdge10 )
{
	return Normalize( Edge10 / tEdge10.x );
}

XMFLOAT3 PrimitiveFactory::CalculateNormal(
	const XMFLOAT3 &Edge10,
	const XMFLOAT3 &Edge20 )
{
	return Normalize( CrossProduct( Edge10, Edge20 ) );
}

XMFLOAT3 PrimitiveFactory::CalculateBiNormal( const XMFLOAT3 & Tangent, const XMFLOAT3 & Normal )
{
	return CrossProduct( Tangent, Normal );
}

void PrimitiveFactory::Common( const ModelSpecs_L & Specs )
{
	PrimitiveFactory::indices.resize( vertices.size() );
	for ( int i = 0; i < vertices.size(); ++i )
	{
		indices[ i ] = i;
	}

	// Load the orienation into SIMD register and create the rotation matrix
	auto rotationVector = ConvertToRadians( XMLoadFloat3( &Specs.orientation ) );
	auto rotationMatrix = XMMatrixRotationRollPitchYawFromVector( rotationVector );

	// Rotate each vertex
	for ( int i = 0; i < vertices.size(); ++i )
	{
		auto &vertex = vertices[ i ];
		auto &normal = normals[ i ];

		auto xmPosition = XMLoadFloat3( &vertex );
		auto xmNormal = XMLoadFloat3( &normal );

		auto rotatedPosition = XMVector3Transform( xmPosition, rotationMatrix );
		auto rotatedNormal = XMVector3Transform( xmNormal, rotationMatrix );

		XMStoreFloat3( &vertex, rotatedPosition );
		XMStoreFloat3( &normal, rotatedNormal );
	}
}

void PrimitiveFactory::ClearAllBuffers()
{
	PrimitiveFactory::vertices.clear();
	PrimitiveFactory::normals.clear();
	PrimitiveFactory::uvs.clear();
	PrimitiveFactory::indices.clear();
	PrimitiveFactory::tangents.clear();
	PrimitiveFactory::binormals.clear();
}

std::vector<DirectX::XMFLOAT3> PrimitiveFactory::vertices;
std::vector<DirectX::XMFLOAT3> PrimitiveFactory::normals;
std::vector<DirectX::XMFLOAT2> PrimitiveFactory::uvs;
std::vector<DirectX::XMFLOAT3> PrimitiveFactory::tangents;
std::vector<DirectX::XMFLOAT3> PrimitiveFactory::binormals;

std::vector<DWORD> PrimitiveFactory::indices;
DirectX::XMFLOAT4 PrimitiveFactory::color;