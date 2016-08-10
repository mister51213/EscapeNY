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
	auto CreateVertexList = [&Specs]()
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
	auto CreateVertexList = [&Specs]()
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
			{ 0.f,0.f,-1.f },
			{ 0.f,0.f,-1.f },
			{ 0.f,0.f,-1.f },
			{ 0.f,0.f,-1.f },
			{ 0.f,0.f,-1.f }
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

void PrimitiveFactory::CreateCube( const ModelSpecs_L &Specs )
{    
	auto CreateVertexList = [&Specs]()->std::vector<DirectX::XMFLOAT3>
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
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y,Specs.center.z - extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y - extentHalf.y,Specs.center.z - extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y,Specs.center.z + extentHalf.z },
			{ Specs.center.x - extentHalf.x, Specs.center.y - extentHalf.y,Specs.center.z + extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y - extentHalf.y,Specs.center.z - extentHalf.z },
			{ Specs.center.x + extentHalf.x, Specs.center.y - extentHalf.y,Specs.center.z + extentHalf.z },

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

void PrimitiveFactory::Common( const ModelSpecs_L & Specs )
{
	PrimitiveFactory::indices.resize( vertices.size() );
	for( int i = 0; i < vertices.size(); ++i )
	{
		indices[ i ] = i;
	}

	// Load the orienation into SIMD register and create the rotation matrix
	auto rotationVector = ConvertToRadians( XMLoadFloat3( &Specs.orientation ) );
	auto rotationMatrix = XMMatrixRotationRollPitchYawFromVector( rotationVector );

	// Rotate each vertex
	for( int i = 0; i < vertices.size(); ++i )
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
}

std::vector<DirectX::XMFLOAT3> PrimitiveFactory::vertices;
std::vector<DirectX::XMFLOAT3> PrimitiveFactory::normals;
std::vector<DirectX::XMFLOAT2> PrimitiveFactory::uvs; 
std::vector<DWORD> PrimitiveFactory::indices;
DirectX::XMFLOAT4 PrimitiveFactory::color;