#include "Utilities.h"
#include "MathUtils.h"

using namespace DirectX;

using PartFn = int( *)( XMFLOAT3*, const int, const int );

int PartitionY( XMFLOAT3* Array, const int Left, const int Right )
{
	const auto mid = Left + ( Right - Left ) / 2;
	const auto pivot = Array[ mid ].y;
	// move the mid point value to the front.
	std::swap( Array[ mid ], Array[ Left ] );
	int i = Left + 1;
	int j = Right;
	while ( i <= j )
	{
		while ( i <= j && Array[ i ].y <= pivot )
		{
			i++;
		}

		while ( i <= j && Array[ j ].y > pivot )
		{
			j--;
		}

		if ( i < j )
		{
			std::swap( Array[ i ], Array[ j ] );
		}
	}
	std::swap( Array[ i - 1 ], Array[ Left ] );
	return i - 1;
}

int PartitionZ( XMFLOAT3* Array, const int Left, const int Right )
{
	const auto mid = Left + ( Right - Left ) / 2;
	const auto pivot = Array[ mid ].z;
	// move the mid point value to the front.
	std::swap( Array[ mid ], Array[ Left ] );
	int i = Left + 1;
	int j = Right;
	while ( i <= j )
	{
		while ( i <= j && Array[ i ].z <= pivot )
		{
			i++;
		}

		while ( i <= j && Array[ j ].z > pivot )
		{
			j--;
		}

		if ( i < j )
		{
			std::swap( Array[ i ], Array[ j ] );
		}
	}
	std::swap( Array[ i - 1 ], Array[ Left ] );
	return i - 1;
}

int PartitionX( XMFLOAT3* Array, const int Left, const int Right )
{
	const auto mid = Left + ( Right - Left ) / 2;
	const auto pivot = Array[ mid ].x;
	// move the mid point value to the front.
	std::swap( Array[ mid ], Array[ Left ] );
	int i = Left + 1;
	int j = Right;
	while ( i <= j )
	{
		while ( i <= j && Array[ i ].x <= pivot )
		{
			i++;
		}

		while ( i <= j && Array[ j ].x > pivot )
		{
			j--;
		}

		if ( i < j )
		{
			std::swap( Array[ i ], Array[ j ] );
		}
	}
	std::swap( Array[ i - 1 ], Array[ Left ] );
	return i - 1;
}

void Quicksort( XMFLOAT3 *arr, const int left, const int right, PartFn Fn ) // function pointer (polymorphic)
{
	if ( left >= right )
	{
		return;
	}

	int part = Fn( arr, left, right );

	Quicksort( arr, left, part - 1, Fn );
	Quicksort( arr, part + 1, right, Fn );
}

void Sort( std::vector<XMFLOAT3> &V )
{
	
	// Erase duplicate vertices from vertex list
	/*for ( int j = V.size() - 1; j >= 0; --j )
	{
		for ( int i = j - 1; i >= 0; --i )
		{
			if ( V[ i ] == V[ j ] )
			{
				V.erase( V.begin() + i );
			}
		}
	}*/
	
	
	// Sort by Y
	Quicksort( V.data(), 0, V.size() - 1, PartitionY );

	// Sort by Z
	float Y = V[ 0 ].y;
	int yStart = 0;	

	for ( int i = 0; i < V.size(); ++i )
	{
		if ( V[ i ].y != Y )
		{
			const int yEnd = i - 1;
			Quicksort( V.data(), yStart, yEnd, PartitionZ );
			yStart = i;
			Y = V[ yStart ].y;
		}
	}

	// Sort by X
	float Z = V[ 0 ].z;
	int zStart = 0;

	for ( int i = 0; i < V.size(); ++i )
	{
		if ( V[ i ].z != Z )
		{
			const int zEnd = i - 1;
			Quicksort( V.data(), zStart, zEnd, PartitionX );
			zStart = i;
			Z = V[ zStart ].z;
		}
	}
	int a = 0;
}

DirectX::XMMATRIX GetWorldMatrix( const ModelSpecs_W & modSpecs )
{
	auto xmPosition = XMLoadFloat3( &modSpecs.position );
	auto xmOrientation = XMLoadFloat3( &modSpecs.orientation );
	xmOrientation = ConvertToRadians( xmOrientation );
	auto xmScale = XMLoadFloat3( &modSpecs.scale );

	DirectX::XMMATRIX trans = XMMatrixTranslationFromVector( xmPosition );
	DirectX::XMMATRIX rot = 
		XMMatrixRotationRollPitchYawFromVector( xmOrientation );
	DirectX::XMMATRIX scal = XMMatrixScalingFromVector( xmScale );

	// Also transpose it in this step to make it easier for GPU to handle
	return XMMatrixTranspose( rot*scal*trans );
}

DirectX::XMFLOAT2 ConvertTileCoordinates( int X, int Y )
{
	return{ ( X * g_tileWidth ) + g_offset, ( Y * g_tileWidth ) + g_offset };
}

void Transpose( std::vector<XMMATRIX>& matrices )
{
	for( auto &matrix : matrices )
	{
		matrix = XMMatrixTranspose( matrix );
	}
}

ModelSpecs_L::ModelSpecs_L( const XMFLOAT3 & Center, const XMFLOAT3 & Orientation, const XMFLOAT3 & Size )
	:
	center( Center ),
	orientation( Orientation ),
	size( Size )
{
}

////////////////////////////////////////////////////
// TODO: Make generic function
// CreateCBufferDescription(UINT size)
// TODO: Make generic function CreateVertexBuffer(??)
/////////////////////////////////////////////////////

AlignedAxisBoundingBox::AlignedAxisBoundingBox( const DirectX::XMFLOAT3 & Center, const DirectX::XMFLOAT3 & Extent )
	:
	center( Center ),
	extent( Extent )
{}

// Assumes that translation has already been applied

bool AlignedAxisBoundingBox::Overlaps( const AlignedAxisBoundingBox & AABB ) const
{
	// Max extents
	const XMFLOAT3 RightTopBackA = ( center + extent );
	const XMFLOAT3 RightTopBackB = ( AABB.center + AABB.extent );

	// Min extent
	const XMFLOAT3 LeftBottomFrontA = ( center - extent );
	const XMFLOAT3 LeftBottomFrontB = ( AABB.center - AABB.extent );

	// Determine if any of the sides overlaps
	const bool xOverlap = ( RightTopBackA.x > RightTopBackB.x &&
							LeftBottomFrontA.x < LeftBottomFrontB.x );
	const bool yOverlap = ( RightTopBackA.y > RightTopBackB.y &&
							LeftBottomFrontA.y < LeftBottomFrontB.y );
	const bool zOverlap = ( RightTopBackA.z > RightTopBackB.z &&
							LeftBottomFrontA.z < LeftBottomFrontB.z );

	// AND the overlap results together 
	const bool doesOverlap = ( xOverlap && yOverlap && zOverlap );

	// Return the final result
	return doesOverlap;

}

PhysAttributes::PhysAttributes(DirectX::XMFLOAT3 cMass)
{
		//momentum = velocity * mass;
		centerOfMass = cMass; // TODO: actor to pass this in in constructor
}

AABB::AABB( const ModelSpecs_L& localSpecs)
{
	m_extentHalf = localSpecs.size * .5f;

	m_max =
	{ localSpecs.center.x + m_extentHalf.x,
		localSpecs.center.y + m_extentHalf.y,
		localSpecs.center.z + m_extentHalf.z };

	m_min =
	{ localSpecs.center.x - m_extentHalf.x,
		localSpecs.center.y - m_extentHalf.y,
		localSpecs.center.z - m_extentHalf.z };
}
