/***********************************************************************************
File: PrimitiveFactory.cpp
Function: Create basic 3d primitives using vector math. Can be combined with
texture functionality for more detailed graphics.

***************************************************************************************/#pragma once

#include "Includes.h"
#include "Utilities.h"

// Contains position, texture, and normal vectors 
// corresponding to mesh file format
struct ModelType
{
	float x, y, z; // vertex coordinates
	float tu, tv; // UV coordinates
	float nx, ny, nz; // normal vector
};

class PrimitiveFactory
{
public:
	// Take center, size, and orientation of primitive to be created
	static void CreateTriangle( const ModelSpecs_L &Specs );
	static void CreatePlane( const ModelSpecs_L &Specs );
	static void CreatePlaneNM( const ModelSpecs_L &Specs );
	static void CreateCube( const ModelSpecs_L &Specs );
	static void CreateSphereNM( const ModelSpecs_L & Specs, const float radiusGlobe = 50.0f );
	static void CreateCubeNM( const ModelSpecs_L &Specs );
	static void CreateMeshNM( const std::wstring&Filename );
	static void CreateColor( float R, float G, float B, float A );
	static void CreateMesh( const std::wstring& fileName );

	// ACCESSOR FUNCTIONS
	static std::vector<DirectX::XMFLOAT3> GetVertices();
	static std::vector<DirectX::XMFLOAT3> GetNormals();
	static std::vector<DirectX::XMFLOAT2> GetUVs();
	static std::vector<DirectX::XMFLOAT3> GetTangent();
	static std::vector<DirectX::XMFLOAT3> GetBiTangent();
	static std::vector<DWORD> GetIndices();
	static DirectX::XMFLOAT4 GetColor();

private:
	static DirectX::XMFLOAT3 CalculateTangent(
		const DirectX::XMFLOAT3 &Edge10,
		const DirectX::XMFLOAT2& tEdge10 );
	static DirectX::XMFLOAT3 CalculateNormal(
		const DirectX::XMFLOAT3 &Edge10,
		const DirectX::XMFLOAT3 &Edge20 );
	static DirectX::XMFLOAT3 CalculateBiNormal(
		const DirectX::XMFLOAT3 &Tangent,
		const DirectX::XMFLOAT3 &Normal );
	static void Common( const ModelSpecs_L &Specs );
	static void ClearAllBuffers();
private:
	static std::vector<DirectX::XMFLOAT3> vertices;
	static std::vector<DirectX::XMFLOAT3> normals;
	static std::vector<DirectX::XMFLOAT2> uvs;
	// Tangent runs parallel to surface in the U coord of texture 
	// coordinates UV
	static std::vector<DirectX::XMFLOAT3> tangents;
	// Suppose to be called binormal, runs parallel to surface in the V
	// coord of the texture coordinates UV
	static std::vector<DirectX::XMFLOAT3> binormals;

	static std::vector<DWORD> indices;
	static DirectX::XMFLOAT4 color;
};

