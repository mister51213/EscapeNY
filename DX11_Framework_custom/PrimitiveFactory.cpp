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

/*
http://www.rastertek.com/dx11tut07.html
*/
// Custom file format - 
// Each line contains position vector(x, y, z), 
// texture coordinates(tu, tv), 
// and normal vector(nx, ny, nz). 
void PrimitiveFactory::CreateMesh(
    const ModelSpecs_L & Specs,
    const ModelType & type, 
    const string& fileName)
{
    ////////////////////////////////  
    // LOAD MODEL FILE (use lambda)
    ////////////////////////////////
   	ID3D11Buffer *pVertexBuffer, *pIndexBuffer;
	int vertexCount, indexCount;
    ModelType* pModel;

	bool result = [&fileName, &vertexCount, &indexCount, &pModel]()
	{
        ifstream fin;
        char input;
        int i;

        // Open the model file.
        fin.open(fileName);

        // If it could not open the file then exit.
        if (fin.fail())
        {
            return false;
        }

        // Read up to the value of vertex count.
        fin.get(input);
        while (input != ':')
        {
            fin.get(input);
        }

        // Read in the vertex count.
        fin >> vertexCount;

        // Set the number of indices to be the same as the vertex count.
        indexCount = vertexCount;

        // Create the model using the vertex count that was read in.
        pModel = new ModelType[vertexCount];
        if (!pModel)
        {
            return false;
        }

        // Read up to the beginning of the data.
        fin.get(input);
        while (input != ':')
        {
            fin.get(input);
        }
        fin.get(input);
        fin.get(input);

        // Read in the vertex data.
        for (i = 0; i < vertexCount; i++)
        {
            fin >> pModel[i].x >> pModel[i].y >> pModel[i].z;
            fin >> pModel[i].tu >> pModel[i].tv;
            fin >> pModel[i].nx >> pModel[i].ny >> pModel[i].nz;
        }

        // Close the model file.
        fin.close();

        return true;
    };

    ///////////////////////////////////////  
    // LOAD MODEL DATA into vertex array //
    ///////////////////////////////////////
    // Load the vertex array and index array with data.
    VertexType* pVertices;
   	unsigned long* pIndices;

	for(int i =0; i< vertexCount; i++)
	{
		pVertices[i].position = XMFLOAT3(pModel[i].x, pModel[i].y, pModel[i].z);
		pVertices[i].texture = XMFLOAT2(pModel[i].tu, pModel[i].tv);
		pVertices[i].normal = XMFLOAT3(pModel[i].nx, pModel[i].ny, pModel[i].nz);

		pIndices[i] = i;
	}


    // Release all 3 arrays
if(pIndices)
	{
		delete [] pIndices;
		pIndices = 0;
	}
if(pVertices)
	{
		delete [] pVertices;
		pVertices = 0;
	}
if(pModel)
	{
		delete [] pModel;
		pModel = 0;
	}

    // TODO: Is it necessary to make the above lambdas and call them here?
	//PrimitiveFactory::ClearAllBuffers();
	//PrimitiveFactory::vertices = CreateVertexList();
	//PrimitiveFactory::normals = CreateNormalsList();
	//PrimitiveFactory::uvs = CreateUVList();
	//Common( Specs );
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
	for( auto &vertex : vertices )
	{
		auto xmVector = XMLoadFloat3( &vertex );
		auto rotatedVector = XMVector3TransformCoord( xmVector, rotationMatrix );
		XMStoreFloat3( &vertex, rotatedVector );
	}

	for( auto &normal : normals )
	{
		auto xmVector = XMLoadFloat3( &normal );
		auto rotatedVector = XMVector3TransformCoord( xmVector, rotationMatrix );
		XMStoreFloat3( &normal, rotatedVector );
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