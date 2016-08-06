/***********************************************************************************
File: PrimitiveFactory.cpp
Function: Create basic 3d primitives using vector math. Can be combined with 
texture functionality for more detailed graphics.

***************************************************************************************/#pragma once

#include "Includes.h"

class PrimitiveFactory
{
    //// prevent making instances of this class
    //PrimitiveFactory() = delete;

public:
	// TODO: Should we keep as is, or use ModelSpecs_L struct instead to
	// TODO: to initialize these primitives?
    // Take center, size, and orientation of primitive to be created
	static void CreateTriangle( const ModelSpecs_L &Specs );
	static void CreatePlane( const ModelSpecs_L &Specs );
	static void CreateCube( const ModelSpecs_L &Specs );
	static void PrimitiveFactory::CreateColor( float R, float G, float B, float A );

    // ACCESSOR FUNCTIONS
	static std::vector<DirectX::XMFLOAT3> GetVertices();
	static std::vector<DirectX::XMFLOAT3> GetNormals();
	static std::vector<DirectX::XMFLOAT2> GetUVs();
	static std::vector<DWORD> GetIndices();
    static DirectX::XMFLOAT4 GetColor();

private:
	 static std::vector<DirectX::XMFLOAT3> vertices;
	 static std::vector<DirectX::XMFLOAT3> normals;
	 static std::vector<DirectX::XMFLOAT2> uvs; 
	 static std::vector<DWORD> indices;
   	 static DirectX::XMFLOAT4 color;
};

