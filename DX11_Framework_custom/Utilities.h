#pragma once
#include <vector>
#include <DirectXMath.h>

// Permit me to be a little lazy :)
// This is just an alias to shorten the amount of typing
template<class T>
using comptr = Microsoft::WRL::ComPtr<T>;

// Helper macros to decrease the amount of typing and space
#define RETURN_IF_FALSE(Result)\
if( !( Result ) ) \
{ \
	ShowCursor(TRUE);\
	return false; \
}\
else int a = 0
#define RETURN_MESSAGE_IF_FALSE(Result, Message)\
if(!(Result))\
{\
	ShowCursor(TRUE);\
	MessageBox(nullptr, Message, L"Error!", MB_OK);\
	return false;\
}\
else int a = 0
#define RETURN_IF_FAILED(Result) RETURN_IF_FALSE( SUCCEEDED( ( Result ) ) )

// Common vertex buffer types and corresponding input element descriptions

// Position and Color 
struct VertexPositionColorType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
	static std::vector<D3D11_INPUT_ELEMENT_DESC> CreateLayoutDescriptions()
	{
		///////////////////////////////////////////////////////////////////////////////////
		// Create the layout of the VERTEX DATA that will be processed by the shader.    //
		// We indicate the usage of each element in the layout to the shader by labeling //
		// the first one POSITION and the second one COLOR.                              //
		///////////////////////////////////////////////////////////////////////////////////

		std::vector<D3D11_INPUT_ELEMENT_DESC> eDesc( 2 );
		eDesc[ 0 ].SemanticName = "POSITION";
		eDesc[ 0 ].SemanticIndex = 0;
		eDesc[ 0 ].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		eDesc[ 0 ].InputSlot = 0;
		eDesc[ 0 ].AlignedByteOffset = 0;
		eDesc[ 0 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		eDesc[ 0 ].InstanceDataStepRate = 0;

		eDesc[ 1 ].SemanticName = "COLOR";
		eDesc[ 1 ].SemanticIndex = 0;
		eDesc[ 1 ].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		eDesc[ 1 ].InputSlot = 0;
		eDesc[ 1 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		eDesc[ 1 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		eDesc[ 1 ].InstanceDataStepRate = 0;

		return eDesc;
	}
};

// Position and texture coordinates
struct VertexPositionUVType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 uv;

	// Input layout descriptions for position and texture coordinates
	static std::vector<D3D11_INPUT_ELEMENT_DESC> CreateLayoutDescriptions()
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> eDesc( 2 );
		eDesc[ 0 ].SemanticName = "POSITION";
		eDesc[ 0 ].SemanticIndex = 0;
		eDesc[ 0 ].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		eDesc[ 0 ].InputSlot = 0;
		eDesc[ 0 ].AlignedByteOffset = 0;
		eDesc[ 0 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		eDesc[ 0 ].InstanceDataStepRate = 0;

		eDesc[ 1 ].SemanticName = "TEXCOORD";
		eDesc[ 1 ].SemanticIndex = 0;
		eDesc[ 1 ].Format = DXGI_FORMAT_R32G32_FLOAT;
		eDesc[ 1 ].InputSlot = 0;
		eDesc[ 1 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		eDesc[ 1 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		eDesc[ 1 ].InstanceDataStepRate = 0;

		return eDesc;
	}

};

// Position, normal and texture coordinates
struct VertexPositionUVNormalType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
	
	// Input layout descriptions for position, normal and texture coordinates
	static std::vector<D3D11_INPUT_ELEMENT_DESC> CreateLayoutDescriptions()
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> eDesc( 2 );
		eDesc[ 0 ].SemanticName = "POSITION";
		eDesc[ 0 ].SemanticIndex = 0;
		eDesc[ 0 ].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		eDesc[ 0 ].InputSlot = 0;
		eDesc[ 0 ].AlignedByteOffset = 0;
		eDesc[ 0 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		eDesc[ 0 ].InstanceDataStepRate = 0;

		eDesc[ 1 ].SemanticName = "NORMAL";
		eDesc[ 1 ].SemanticIndex = 0;
		eDesc[ 1 ].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		eDesc[ 1 ].InputSlot = 0;
		eDesc[ 1 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		eDesc[ 1 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		eDesc[ 1 ].InstanceDataStepRate = 0;

		eDesc[ 2 ].SemanticName = "TEXCOORD";
		eDesc[ 2 ].SemanticIndex = 0;
		eDesc[ 2 ].Format = DXGI_FORMAT_R32G32_FLOAT;
		eDesc[ 2 ].InputSlot = 0;
		eDesc[ 2 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		eDesc[ 2 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		eDesc[ 2 ].InstanceDataStepRate = 0;

		return eDesc;
	}
};