/***********************************************************************************
File: PrimitiveFactory.cpp
Function: Create basic 3d primitives using vector math. Can be combined with 
texture functionality for more detailed graphics.

***************************************************************************************/#pragma once

#include "Includes.h"
#include "Utilities.h"
#include <fstream>
#include <string>

using namespace std;


class PrimitiveFactory
{
public:
	static void CreateTriangle( const ModelSpecs_L &Specs );
	static void CreatePlane( const ModelSpecs_L &Specs );
	static void CreateCube( const ModelSpecs_L &Specs );
	static void PrimitiveFactory::CreateColor( float R, float G, float B, float A );
    static void PrimitiveFactory::CreateMesh( const wstring& fileName );

    // ACCESSOR FUNCTIONS
	static std::vector<DirectX::XMFLOAT3> GetVertices();
	static std::vector<DirectX::XMFLOAT3> GetNormals();
	static std::vector<DirectX::XMFLOAT2> GetUVs();
	static std::vector<DWORD> GetIndices();
    static DirectX::XMFLOAT4 GetColor();

private:
	static void Common( const ModelSpecs_L &Specs );
	static void ClearAllBuffers();
private:
	 static std::vector<DirectX::XMFLOAT3> vertices;
	 static std::vector<DirectX::XMFLOAT3> normals;
	 static std::vector<DirectX::XMFLOAT2> uvs; 
	 static std::vector<DWORD> indices;
	 static DirectX::XMFLOAT4 color;
};

