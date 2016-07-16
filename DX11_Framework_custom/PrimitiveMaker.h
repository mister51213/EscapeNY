#pragma once

#include "Includes.h"

class PrimitiveMaker
{
public:
	PrimitiveMaker();
	~PrimitiveMaker();

    // All of these functions take center, size, and orientation of the primitive to be created
	void CreateTriangle( const DirectX::XMFLOAT3 &Center, const DirectX::XMFLOAT2 &Extent, const DirectX::XMFLOAT3 &Orientation = {0.f, 0.f, 0.f} );
	void CreatePlane( const DirectX::XMFLOAT3 &Center, const DirectX::XMFLOAT2 &Extent,
		const DirectX::XMFLOAT3 &Orientation = {0.f, 0.f, 0.f} );
	void CreateCube( const DirectX::XMFLOAT3 &Center, const DirectX::XMFLOAT3 &Extent,
		const DirectX::XMFLOAT3 &Orientation = {0.f, 0.f, 0.f} );
	DirectX::XMFLOAT4 CreateColor( float R, float G, float B, float A );

	std::vector<DirectX::XMFLOAT3> GetVertices()const;
	std::vector<DirectX::XMFLOAT3> GetNormals()const;
	std::vector<DirectX::XMFLOAT2> GetUVs()const;
	std::vector<DWORD> GetIndices()const;

private:
	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<DirectX::XMFLOAT3> normals;
    // texture uv coordinates (x and y) 
	std::vector<DirectX::XMFLOAT2> uvs; 
	std::vector<DWORD> indices;
};

