#include "PrimitiveMaker.h"

using namespace DirectX;

PrimitiveMaker::PrimitiveMaker()
{}


PrimitiveMaker::~PrimitiveMaker()
{}

void PrimitiveMaker::CreateTriangle( const DirectX::XMFLOAT3 & Center, const DirectX::XMFLOAT2 & Extent )
{
	vertices = 
	{
		{Center.x - Extent.x, Center.y - Extent.y, Center.z},
		{Center.x, Extent.y, Center.z},
		{Center.x + Extent.x, Center.y - Extent.y, Center.z}
	};

	normals = 
	{
		{0.f, 0.f, -1.f},
		{0.f, 0.f, -1.f},
		{0.f, 0.f, -1.f}
	};

	uvs = 
	{
		{0.f, 1.f},
		{0.5f, 0.f},
		{1.f, 1.f}
	};
}

void PrimitiveMaker::CreatePlane( const DirectX::XMFLOAT3 & Center, const DirectX::XMFLOAT2 & Extent, const DirectX::XMFLOAT3 & Orientation )
{
	// There needs to be as many normals and uvs as there are vertices, 
	// there can be more indices

	// Create vertex list
	vertices = 
	{
		{Center.x - Extent.x, Center.y + Extent.y, Center.z},
		{Center.x + Extent.x, Center.y + Extent.y, Center.z},
		{Center.x - Extent.x, Center.y - Extent.y, Center.z},
		{Center.x + Extent.x, Center.y - Extent.y, Center.z}
	};
	
	// Create normals list
	normals =
	{
		{0.f,0.f,-1.f},
		{0.f,0.f,-1.f}, 
		{0.f,0.f,-1.f},
		{0.f,0.f,-1.f}
	};

	// Create uv coordinate list
	uvs = 
	{ 
		{0.f, 0.f}, 
		{1.f, 0.f}, 
		{0.f, 1.f}, 
		{1.f, 1.f} 
	};

	// This is for a D3D11_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, for a 
	// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP you would only need 0,1,2,3
	indices = 
	{
		{0},{1},{2},{2},{1},{3}
	};

	// Load the orienation into SIMD register and create the rotation matrix
	auto rotationVector = XMLoadFloat3( &Orientation );
	auto rotationMatrix = XMMatrixRotationRollPitchYawFromVector( rotationVector );

	// Rotate each vertex
	for( auto &vertex : vertices )
	{
		auto xmVector = XMLoadFloat3( &vertex );
		auto rotatedVector = XMVector3TransformCoord( xmVector, rotationMatrix );
		XMStoreFloat3( &vertex, rotatedVector );		
	}

	// Load the reverse orientation into SIMD register and create the reverse rotation matrix
	// for the normals.  This makes sure that lights don't follow the direction of the 
	// plane, but instead go in the opposite direction
	auto reverseRotationMatrix = XMMatrixRotationRollPitchYawFromVector( -rotationVector );
	for( auto &normal : normals )
	{
		auto xmVector = XMLoadFloat3( &normal );
		auto rotatedVector = XMVector3TransformCoord( xmVector, reverseRotationMatrix );
		XMStoreFloat3( &normal, rotatedVector );
	}

}

void PrimitiveMaker::CreateCube( const DirectX::XMFLOAT3 & Center, const DirectX::XMFLOAT3 & Extent, const DirectX::XMFLOAT3 & Orientation )
{
	vertices = 
	{
		// Left
		{Center.x - Extent.x, Center.y + Extent.y, Center.z + Extent.z},
		{Center.x - Extent.x, Center.y + Extent.y, Center.z - Extent.z},
		{Center.x - Extent.x, Center.y - Extent.y, Center.z + Extent.z},
		{Center.x - Extent.x, Center.y - Extent.y, Center.z + Extent.z},
		{Center.x - Extent.x, Center.y + Extent.y, Center.z - Extent.z},
		{Center.x - Extent.x, Center.y - Extent.y, Center.z - Extent.z},

		// Right
		{Center.x + Extent.x, Center.y + Extent.y, Center.z - Extent.z},
		{Center.x + Extent.x, Center.y + Extent.y, Center.z + Extent.z},
		{Center.x + Extent.x, Center.y - Extent.y, Center.z - Extent.z},
		{Center.x + Extent.x, Center.y - Extent.y, Center.z - Extent.z},
		{Center.x + Extent.x, Center.y + Extent.y, Center.z + Extent.z},
		{Center.x + Extent.x, Center.y - Extent.y, Center.z + Extent.z},

		// Bottom
		{Center.x - Extent.x, Center.y - Extent.y, Center.z - Extent.z},
		{Center.x + Extent.x, Center.y - Extent.y, Center.z - Extent.z},
		{Center.x - Extent.x, Center.y - Extent.y, Center.z + Extent.z},
		{Center.x - Extent.x, Center.y - Extent.y, Center.z + Extent.z},
		{Center.x + Extent.x, Center.y - Extent.y, Center.z - Extent.z},
		{Center.x + Extent.x, Center.y - Extent.y, Center.z + Extent.z},

		// Top
		{Center.x - Extent.x, Center.y + Extent.y, Center.z + Extent.z},
		{Center.x + Extent.x, Center.y + Extent.y, Center.z + Extent.z},
		{Center.x - Extent.x, Center.y + Extent.y, Center.z - Extent.z},
		{Center.x - Extent.x, Center.y + Extent.y, Center.z - Extent.z},
		{Center.x + Extent.x, Center.y + Extent.y, Center.z + Extent.z},
		{Center.x + Extent.x, Center.y + Extent.y, Center.z - Extent.z},

		// Front
		{Center.x - Extent.x, Center.y + Extent.y, Center.z - Extent.z},
		{Center.x + Extent.x, Center.y + Extent.y, Center.z - Extent.z},
		{Center.x - Extent.x, Center.y - Extent.y, Center.z - Extent.z},
		{Center.x - Extent.x, Center.y - Extent.y, Center.z - Extent.z},
		{Center.x + Extent.x, Center.y + Extent.y, Center.z - Extent.z},
		{Center.x + Extent.x, Center.y - Extent.y, Center.z - Extent.z},

		// Back
		{Center.x + Extent.x, Center.y + Extent.y, Center.z + Extent.z},
		{Center.x - Extent.x, Center.y + Extent.y, Center.z + Extent.z},
		{Center.x + Extent.x, Center.y - Extent.y, Center.z + Extent.z},
		{Center.x + Extent.x, Center.y - Extent.y, Center.z + Extent.z},
		{Center.x - Extent.x, Center.y + Extent.y, Center.z + Extent.z},
		{Center.x - Extent.x, Center.y - Extent.y, Center.z + Extent.z}
	};

	normals = 
	{
		// Left
		{-1.f, 0.f, 0.f},
		{-1.f, 0.f, 0.f},
		{-1.f, 0.f, 0.f},
		{-1.f, 0.f, 0.f},
		{-1.f, 0.f, 0.f},
		{-1.f, 0.f, 0.f},

		// Right
		{1.f, 0.f, 0.f},
		{1.f, 0.f, 0.f},
		{1.f, 0.f, 0.f},
		{1.f, 0.f, 0.f},
		{1.f, 0.f, 0.f},
		{1.f, 0.f, 0.f},

		// Bottom
		{0.f, -1.f, 0.f},
		{0.f, -1.f, 0.f},
		{0.f, -1.f, 0.f},
		{0.f, -1.f, 0.f},
		{0.f, -1.f, 0.f},
		{0.f, -1.f, 0.f},

		// Top
		{0.f, 1.f, 0.f},
		{0.f, 1.f, 0.f},
		{0.f, 1.f, 0.f},
		{0.f, 1.f, 0.f},
		{0.f, 1.f, 0.f},
		{0.f, 1.f, 0.f},

		// Front
		{0.f, 0.f, -1.f},
		{0.f, 0.f, -1.f},
		{0.f, 0.f, -1.f},
		{0.f, 0.f, -1.f},
		{0.f, 0.f, -1.f},
		{0.f, 0.f, -1.f},

		// Back
		{0.f, 0.f, 1.f},
		{0.f, 0.f, 1.f},
		{0.f, 0.f, 1.f},
		{0.f, 0.f, 1.f},
		{0.f, 0.f, 1.f},
		{0.f, 0.f, 1.f}
	};

	uvs = 
	{
		// Left
		{0.f, 0.f},
		{1.f, 0.f},
		{0.f, 1.f},
		{0.f, 1.f},
		{1.f, 0.f},
		{1.f, 1.f},
		// Right
		{0.f, 0.f},
		{1.f, 0.f},
		{0.f, 1.f},
		{0.f, 1.f},
		{1.f, 0.f},
		{1.f, 1.f},
		// Bottom
		{0.f, 0.f},
		{1.f, 0.f},
		{0.f, 1.f},
		{0.f, 1.f},
		{1.f, 0.f},
		{1.f, 1.f},
		// Top
		{0.f, 0.f},
		{1.f, 0.f},
		{0.f, 1.f},
		{0.f, 1.f},
		{1.f, 0.f},
		{1.f, 1.f},
		// Front
		{0.f, 0.f},
		{1.f, 0.f},
		{0.f, 1.f},
		{0.f, 1.f},
		{1.f, 0.f},
		{1.f, 1.f},
		// Back
		{0.f, 0.f},
		{1.f, 0.f},
		{0.f, 1.f},
		{0.f, 1.f},
		{1.f, 0.f},
		{1.f, 1.f}
	};

	indices.resize( vertices.size() );
	for( int i = 0; i < vertices.size(); ++i )
	{
		indices[ i ] = i;
	}

	// Load the orienation into SIMD register and create the rotation matrix
	auto rotationVector = XMLoadFloat3( &Orientation );
	auto rotationMatrix = XMMatrixRotationRollPitchYawFromVector( rotationVector );

	// Rotate each vertex
	for( auto &vertex : vertices )
	{
		auto xmVector = XMLoadFloat3( &vertex );
		auto rotatedVector = XMVector3TransformCoord( xmVector, rotationMatrix );
		XMStoreFloat3( &vertex, rotatedVector );
	}

	// Load the reverse orientation into SIMD register and create the reverse rotation matrix
	// for the normals.  This makes sure that lights don't follow the direction of the 
	// plane, but instead go in the opposite direction
	auto reverseRotationMatrix = XMMatrixRotationRollPitchYawFromVector( -rotationVector );
	for( auto &normal : normals )
	{
		auto xmVector = XMLoadFloat3( &normal );
		auto rotatedVector = XMVector3TransformCoord( xmVector, reverseRotationMatrix );
		XMStoreFloat3( &normal, rotatedVector );
	}


}

std::vector<DirectX::XMFLOAT3> PrimitiveMaker::GetVertices() const
{
	return vertices;
}

std::vector<DirectX::XMFLOAT3> PrimitiveMaker::GetNormals() const
{
	return normals;
}

std::vector<DirectX::XMFLOAT2> PrimitiveMaker::GetUVs() const
{
	return uvs;
}

std::vector<DWORD> PrimitiveMaker::GetIndices() const
{
	return indices;
}
