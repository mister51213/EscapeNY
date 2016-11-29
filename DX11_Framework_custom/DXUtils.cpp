#include "DXUtils.h"

using namespace DirectX;

D3D11_BUFFER_DESC MatrixBufferType::CreateMatrixDescription()
{
	D3D11_BUFFER_DESC matrixBufferDesc{};
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof( MatrixBufferType );
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	return matrixBufferDesc;
}


std::vector<D3D11_INPUT_ELEMENT_DESC> VertexBufferTypeAllInOne::CreateLayoutDescriptions()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> desc
	{
		{ "POSITION", 0, float3Format, 0, 0, vertexDataType, 0 },
		{ "TEXCOORD", 0, float2Format, 0, appendAlignment, vertexDataType, 0 },
		{ "NORMAL", 0, float3Format, 0, appendAlignment, vertexDataType, 0 },
		{ "COLOR", 0, float4Format, 0, appendAlignment, vertexDataType, 0 }
	};

	return desc;
}

D3D11_BUFFER_DESC LightBufferType::CreateLightDescription( unsigned int MaxLightCount )
{
	D3D11_BUFFER_DESC lightBufferDesc{};
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof( LightBufferType ) * MaxLightCount;
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	return lightBufferDesc;
}

D3D11_SAMPLER_DESC CreateSamplerDescription()
{
	D3D11_SAMPLER_DESC samplerDesc;
	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[ 0 ] = 0;
	samplerDesc.BorderColor[ 1 ] = 0;
	samplerDesc.BorderColor[ 2 ] = 0;
	samplerDesc.BorderColor[ 3 ] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	return samplerDesc;
}

std::vector<D3D11_INPUT_ELEMENT_DESC> VertexBufferTypeAllInOneNMap::CreateLayoutDescriptions()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> desc
	{
		{ "POSITION", 0, float3Format, 0, 0, vertexDataType, 0 },
		{ "TEXCOORD", 0, float2Format, 0, appendAlignment, vertexDataType, 0 },
		{ "COLOR", 0, float4Format, 0, appendAlignment, vertexDataType, 0 },
		{ "TANGENT", 0, float3Format, 0, appendAlignment, vertexDataType, 0 },
		{ "BINORMAL", 0, float3Format, 0, appendAlignment, vertexDataType, 0 },
		{ "NORMAL", 0, float3Format, 0, appendAlignment, vertexDataType, 0 }
	};

	return desc;
}

D3D11_BUFFER_DESC SceneBufferType::CreateLightDescription()
{
	D3D11_BUFFER_DESC lightBufferDesc{};
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof( SceneBufferType );
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	return lightBufferDesc;
}
