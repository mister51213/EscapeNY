#pragma once

#include "Includes.h"

using namespace DirectX;

struct MatrixBufferType
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;

	static D3D11_BUFFER_DESC CreateMatrixDescription();
};

// Common vertex buffer types and corresponding input element descriptions
constexpr DWORD appendAlignment = D3D11_APPEND_ALIGNED_ELEMENT;
constexpr DXGI_FORMAT float2Format = DXGI_FORMAT_R32G32_FLOAT;
constexpr DXGI_FORMAT float3Format = DXGI_FORMAT_R32G32B32_FLOAT;
constexpr DXGI_FORMAT float4Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
constexpr D3D11_INPUT_CLASSIFICATION vertexDataType = D3D11_INPUT_PER_VERTEX_DATA;

struct VertexBufferTypePosUvNormal
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
};
struct VertexBufferTypeAllInOne
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 color;

	// Input layout descriptions for position, texcoord, normals and color
	static std::vector<D3D11_INPUT_ELEMENT_DESC> CreateLayoutDescriptions();
};

struct VertexBufferTypeAllInOneNMap
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 binormal;
	DirectX::XMFLOAT3 normal;

	// Input layout descriptions for position, texcoord, normals and color
	static std::vector<D3D11_INPUT_ELEMENT_DESC> CreateLayoutDescriptions();
};

// Use these light types so the shader can determine which light calculations
// need to be done and what values it can use.
enum eLightType
{
	INFINITE_LIGHT, POINT_LIGHT, SPOT_LIGHT
};
// Use this one type for ALL different lights
struct LightBufferType
{
	XMFLOAT4 lightColor;
	XMFLOAT3 lightPosition;
	float coneAngle;
	XMFLOAT3 lightDirection;
	eLightType type;

	static D3D11_BUFFER_DESC CreateLightDescription( unsigned int MaxLightCount );
};

// This must match the number of lights in the shader (hlsl) array or
// be less than it.  Can/Will be used to make sure we don't try copying
// more light data than shader is meant to process.
#define MAX_SHADER_LIGHTS 10
struct SceneBufferType
{
	XMFLOAT4 ambientColor;
	int lightCount;
	XMFLOAT3 padding;
	LightBufferType lights[ MAX_SHADER_LIGHTS ];

	static D3D11_BUFFER_DESC CreateLightDescription();
};

D3D11_SAMPLER_DESC CreateSamplerDescription();