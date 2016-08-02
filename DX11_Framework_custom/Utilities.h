#pragma once
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

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

////////////////////////////////////////////////////
// Vector Math Operator Overloads
///////////////////////////////////////////////////

//XMFLOAT3 operator += (XMFLOAT3& v1, XMFLOAT3& v2)
//{
//    v1.x += v2.x;
//    v1.y += v2.y;
//    v1.z += v2.z;
//    return v1;
//}

// redundant?
//XMFLOAT3 operator + (XMFLOAT3& v1, float cubeMod)
//{
//    v1.x += cubeMod;
//    v1.y += cubeMod;
//    v1.z += cubeMod;
//    return v1;
//}

////////////////////////////////////////////////////////////////
// KEY FUNCTION FOR GETTING WORLD MATRIX
// TODO: make it global; see which is faster
////////////////////////////////////////////////////////////////

inline DirectX::XMMATRIX GetWorldMatrix(const XMFLOAT3& translation, const XMFLOAT3& rotate, const XMFLOAT3& scale)
{    
    DirectX::XMMATRIX trans = XMMatrixTranslation(translation.x, translation.y, translation.z);
    DirectX::XMMATRIX rot = XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
    DirectX::XMMATRIX scal = XMMatrixScaling(scale.x, scale.y, scale.z);

    // It matters what order you multiply the translation matrix in
    return XMMatrixTranspose(rot*scal*trans); // Also transpose it in this step to make it easier for GPU to handle
    // TODO: remove transponse world matrix from shader
}

// TODO: Make this global inline in Utilities
// Transpose the matrices to prepare them for the shader.
inline DirectX::XMMATRIX Transpose(std::vector<XMMATRIX> & matrices)
{    
    // TODO:  Figure out best way to do this; what's the fastest way,
    // pass a list of matrices, or what?????

	/*MatrixBufferType data;
	data.world = XMMatrixTranspose( worldMatrix );
	data.view = XMMatrixTranspose( viewMatrix );
	data.projection = XMMatrixTranspose( projectionMatrix );*/
    return XMMatrixIdentity();
}

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