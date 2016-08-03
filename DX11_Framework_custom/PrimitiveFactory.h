/***********************************************************************************
File: PrimitiveFactory.cpp
Function: Create basic 3d primitives using vector math. Can be combined with 
texture functionality for more detailed graphics.

***************************************************************************************/#pragma once

#include "Includes.h"

class PrimitiveFactory
{
public:
	PrimitiveFactory();
	~PrimitiveFactory();

    // All of these functions take center, size, and orientation 
    // of the primitive to be created
	void CreateTriangle( 
        const DirectX::XMFLOAT3 &Center, 
        const DirectX::XMFLOAT2 &Extent, 
        const DirectX::XMFLOAT3 &Orientation = {0.f, 0.f, 0.f} );
	void CreatePlane( 
        const DirectX::XMFLOAT3 &Center, 
        const DirectX::XMFLOAT2 &Extent,
		const DirectX::XMFLOAT3 &Orientation = {0.f, 0.f, 0.f} );
	void CreateCube( 
        const DirectX::XMFLOAT3 &Center, 
        const DirectX::XMFLOAT3 &Extent,
		const DirectX::XMFLOAT3 &Orientation = {0.f, 0.f, 0.f} );
	void PrimitiveFactory::CreateColor( float R, float G, float B, float A );

    // ACCESSOR FUNCTIONS
	std::vector<DirectX::XMFLOAT3> GetVertices()const;
	std::vector<DirectX::XMFLOAT3> GetNormals()const;
	std::vector<DirectX::XMFLOAT2> GetUVs()const;
	std::vector<DWORD> GetIndices()const;
    DirectX::XMFLOAT4 GetColor()const;

private:
	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<DirectX::XMFLOAT3> normals;
    // texture uv coordinates (x and y) 
	std::vector<DirectX::XMFLOAT2> uvs; 
	std::vector<DWORD> indices;
   	DirectX::XMFLOAT4 color;
};

