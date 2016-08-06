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
	auto extentHalf = Specs.size * 0.5f;
    PrimitiveFactory::vertices.clear();
	PrimitiveFactory::vertices = 
	{
		{Center.x - extentHalf.x, Center.y - extentHalf.y, Center.z},
		{Center.x, Center.y + extentHalf.y, Center.z},
		{Center.x + extentHalf.x, Center.y - extentHalf.y, Center.z}
	};

    // Must clear each time because this is static.
    PrimitiveFactory::normals.clear();
	PrimitiveFactory::normals = 
	{
		{0.f, 0.f, -1.f},
		{0.f, 0.f, -1.f},
		{0.f, 0.f, -1.f}
	};
 
    // Must clear each time because this is static.
    PrimitiveFactory::uvs.clear();
	PrimitiveFactory::uvs = 
	{
		{0.f, 1.f},
		{0.5f, 0.f},
		{1.f, 1.f}
	};

    // Must clear each time because this is static.
    PrimitiveFactory::indices.clear();
    // TODO: can resize() resize DOWN too; if so, don't need clear above
	PrimitiveFactory::indices.resize( vertices.size() );
	for( int i = 0; i < vertices.size(); ++i )
	{
		indices[ i ] = i;
	}

	// Load the orienation into SIMD register and create the rotation matrix
	auto rotationVector = ConvertToRadians(XMLoadFloat3( &Orientation ));
	auto rotationMatrix = XMMatrixRotationRollPitchYawFromVector( rotationVector );

	// Rotate each vertex
	for( auto &vertex : vertices )
	{
		auto xmVector = XMLoadFloat3( &vertex );
		auto rotatedVector = XMVector3TransformCoord( xmVector, rotationMatrix );
		XMStoreFloat3( &vertex, rotatedVector );
	}

	// Load the reverse orientation into SIMD register and create the reverse rotation matrix
	// for the normals. This makes sure that lights don't follow the direction of the 
	// plane, but instead go in the opposite direction
	auto reverseRotationMatrix = XMMatrixRotationRollPitchYawFromVector( rotationVector );
	for( auto &normal : normals )
	{
		auto xmVector = XMLoadFloat3( &normal );
		auto rotatedVector = XMVector3TransformCoord( xmVector, reverseRotationMatrix );
		XMStoreFloat3( &normal, rotatedVector );
	}
}

void PrimitiveFactory::CreatePlane( const ModelSpecs_L &Specs )
{
	// There needs to be as many normals and uvs as there are vertices, 
	// there can be more indices

    // Extent half is size of the object in each direction
    auto extentHalf = DirectX::XMFLOAT2( Extent.x * 0.5f, Extent.y * 0.5f );
	// Create vertex list	
    PrimitiveFactory::vertices.clear();
    PrimitiveFactory::vertices = 
	{
		{Center.x - extentHalf.x, Center.y + extentHalf.y, Center.z},
		{Center.x + extentHalf.x, Center.y + extentHalf.y, Center.z},
		{Center.x - extentHalf.x, Center.y - extentHalf.y, Center.z},
		{Center.x - extentHalf.x, Center.y - extentHalf.y, Center.z},
		{Center.x + extentHalf.x, Center.y + extentHalf.y, Center.z},
		{Center.x + extentHalf.x, Center.y - extentHalf.y, Center.z}
	};
	
	// Create normals list
    PrimitiveFactory::normals.clear();
	PrimitiveFactory::normals =
	{
		{0.f,0.f,-1.f},
		{0.f,0.f,-1.f}, 
		{0.f,0.f,-1.f},
		{0.f,0.f,-1.f},
		{0.f,0.f,-1.f},
		{0.f,0.f,-1.f}
	};

	// Create uv coordinate list
    PrimitiveFactory::uvs.clear();
	PrimitiveFactory::uvs = 
	{ 
		{0.f, 0.f}, 
		{1.f, 0.f}, 
		{0.f, 1.f}, 
		{0.f, 1.f},
		{1.f, 0.f},
		{1.f, 1.f} 
	};

	// This is for a D3D11_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, for a 
	// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP you would only need 0,1,2,3
    PrimitiveFactory::indices.clear();
	PrimitiveFactory::indices.resize( vertices.size() );
	for( int i = 0; i < vertices.size(); ++i )
	{
		indices[ i ] = i;
	}

	// Load the orienation into SIMD register and create the rotation matrix
	auto rotationVector = ConvertToRadians(XMLoadFloat3( &Orientation ));
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

void PrimitiveFactory::CreateCube( const ModelSpecs_L &Specs )
{    
    // TODO: Better name this function
    auto extentHalf = DirectX::XMFLOAT3( Size.x * 0.5f, Size.y * 0.5f, Size.z * 0.5f );

    PrimitiveFactory::vertices.clear();
	PrimitiveFactory::vertices = 
	{	
        // Left
		{Center.x - extentHalf.x, Center.y + extentHalf.y, Center.z + extentHalf.z},
		{Center.x - extentHalf.x, Center.y + extentHalf.y, Center.z - extentHalf.z},
		{Center.x - extentHalf.x, Center.y - extentHalf.y, Center.z + extentHalf.z},
		{Center.x - extentHalf.x, Center.y - extentHalf.y, Center.z + extentHalf.z},
		{Center.x - extentHalf.x, Center.y + extentHalf.y, Center.z - extentHalf.z},
		{Center.x - extentHalf.x, Center.y - extentHalf.y, Center.z - extentHalf.z},

		// Right
		{Center.x + extentHalf.x, Center.y + extentHalf.y, Center.z - extentHalf.z},
		{Center.x + extentHalf.x, Center.y + extentHalf.y, Center.z + extentHalf.z},
		{Center.x + extentHalf.x, Center.y - extentHalf.y, Center.z - extentHalf.z},
		{Center.x + extentHalf.x, Center.y - extentHalf.y, Center.z - extentHalf.z},
		{Center.x + extentHalf.x, Center.y + extentHalf.y, Center.z + extentHalf.z},
		{Center.x + extentHalf.x, Center.y - extentHalf.y, Center.z + extentHalf.z},

		// Bottom
		{Center.x - extentHalf.x, Center.y - extentHalf.y, Center.z - extentHalf.z},
		{Center.x + extentHalf.x, Center.y - extentHalf.y, Center.z - extentHalf.z},
		{Center.x - extentHalf.x, Center.y - extentHalf.y, Center.z + extentHalf.z},
		{Center.x - extentHalf.x, Center.y - extentHalf.y, Center.z + extentHalf.z},
		{Center.x + extentHalf.x, Center.y - extentHalf.y, Center.z - extentHalf.z},
		{Center.x + extentHalf.x, Center.y - extentHalf.y, Center.z + extentHalf.z},

		// Top
		{Center.x - extentHalf.x, Center.y + extentHalf.y, Center.z + extentHalf.z},
		{Center.x + extentHalf.x, Center.y + extentHalf.y, Center.z + extentHalf.z},
		{Center.x - extentHalf.x, Center.y + extentHalf.y, Center.z - extentHalf.z},
		{Center.x - extentHalf.x, Center.y + extentHalf.y, Center.z - extentHalf.z},
		{Center.x + extentHalf.x, Center.y + extentHalf.y, Center.z + extentHalf.z},
		{Center.x + extentHalf.x, Center.y + extentHalf.y, Center.z - extentHalf.z},

		// Front
		{Center.x - extentHalf.x, Center.y + extentHalf.y, Center.z - extentHalf.z},
		{Center.x + extentHalf.x, Center.y + extentHalf.y, Center.z - extentHalf.z},
		{Center.x - extentHalf.x, Center.y - extentHalf.y, Center.z - extentHalf.z},
		{Center.x - extentHalf.x, Center.y - extentHalf.y, Center.z - extentHalf.z},
		{Center.x + extentHalf.x, Center.y + extentHalf.y, Center.z - extentHalf.z},
		{Center.x + extentHalf.x, Center.y - extentHalf.y, Center.z - extentHalf.z},

		// Back
		{Center.x + extentHalf.x, Center.y + extentHalf.y, Center.z + extentHalf.z},
		{Center.x - extentHalf.x, Center.y + extentHalf.y, Center.z + extentHalf.z},
		{Center.x + extentHalf.x, Center.y - extentHalf.y, Center.z + extentHalf.z},
		{Center.x + extentHalf.x, Center.y - extentHalf.y, Center.z + extentHalf.z},
		{Center.x - extentHalf.x, Center.y + extentHalf.y, Center.z + extentHalf.z},
		{Center.x - extentHalf.x, Center.y - extentHalf.y, Center.z + extentHalf.z}
	};

    PrimitiveFactory::normals.clear();
	PrimitiveFactory::normals = 
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

    PrimitiveFactory::uvs.clear();
	PrimitiveFactory::uvs = 
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

    PrimitiveFactory::indices.clear();
	PrimitiveFactory::indices.resize( vertices.size() );
	for( int i = 0; i < vertices.size(); ++i )
	{
		indices[ i ] = i;
	}

	// Load the orienation into SIMD register and create the rotation matrix
	auto rotationVector = ConvertToRadians(XMLoadFloat3( &Orientation ));
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

std::vector<DWORD> PrimitiveFactory::GetIndices() 
{
	return PrimitiveFactory::indices;
}

DirectX::XMFLOAT4 PrimitiveFactory::GetColor() 
{
	return PrimitiveFactory::color;
}

std::vector<DirectX::XMFLOAT3> PrimitiveFactory::vertices;
std::vector<DirectX::XMFLOAT3> PrimitiveFactory::normals;
std::vector<DirectX::XMFLOAT2> PrimitiveFactory::uvs; 
std::vector<DWORD> PrimitiveFactory::indices;
DirectX::XMFLOAT4 PrimitiveFactory::color;