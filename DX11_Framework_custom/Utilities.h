#pragma once
#include <vector>
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d11.h>

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
// Math Constants
///////////////////////////////////////////////////

// PI is equal to 180 degrees
constexpr float PI = 3.141592654f;

// TwoPI is equal to 360 degrees
constexpr float TwoPI = 2.f * PI;

// PIDivFour is equal to 45 degrees
constexpr float PIDivFour = PI / 4.f;

// PIDivTwo is equal to 90 degrees
constexpr float PIDivTwo = PI / 2.f;

// Radian is 0.01745329238f radians, multiply degrees by radian to convert 
// degrees to radians for rotation functions
constexpr float radian = PI / 180.f;

// Radian to degree conversion
constexpr float degree = 180.f / PI;

////////////////////////////////////////////////////
// Vector Math Operator Overloads
///////////////////////////////////////////////////

// Keep operator overloads out of namespace so we can access them
// without having to declare: using namespace

XMFLOAT3 operator+( const XMFLOAT3 &V1, const XMFLOAT3& V2 );
XMFLOAT3 operator-( const XMFLOAT3 &V1, const XMFLOAT3& V2 );
XMFLOAT3 operator*( const XMFLOAT3 &V, const float S );
XMFLOAT3 operator/( const XMFLOAT3 &V, const float S );

XMFLOAT3 &operator+= ( XMFLOAT3& V1, const XMFLOAT3& V2 );
XMFLOAT3 &operator-= ( XMFLOAT3& V1, const XMFLOAT3& V2 );
XMFLOAT3 &operator*= ( XMFLOAT3 &V, const float S );
XMFLOAT3 &operator/= ( XMFLOAT3 &V, const float S );

// Calculates the cross-product of two float3 vectors
XMFLOAT3 CrossProduct( const XMFLOAT3 &V1, const XMFLOAT3 &V2 );

// Calculates the dot-product of two float3 vectors
float DotProduct( const XMFLOAT3 &V1, const XMFLOAT3 &V2 );

// Calculates the magnitude of a float3 vector
float Magnitude( const XMFLOAT3 &V );

// Calcualtes the length between two float3 point vectors
float Length( const XMFLOAT3 &V1, const XMFLOAT3 &V2 );

// Normalizes a float3 vector
XMFLOAT3 Normalize( const XMFLOAT3 &V );

XMVECTOR ConvertToRadians( const XMVECTOR& angleInDegrees );
XMFLOAT3 ConvertToRadians( const XMFLOAT3& angleInDegrees );
float CalculateYRotation( const float X, const float Z, float &RX, float &RZ );

////////////////////////////////////////////////////////////////
// STRUCTS CONTAINING MODEL INFO for gameplay and rendering
////////////////////////////////////////////////////////////////
struct ModelSpecs_W
{
	XMFLOAT3 position, orientation, scale;
};

struct ModelSpecs_L
{
	ModelSpecs_L(
		const XMFLOAT3 &Center = { 0.f, 0.f, 0.f },
		const XMFLOAT3 &Orientation = { 0.f,0.f,0.f },
		const XMFLOAT3 &Size = { 5.f, 5.f, 5.f } );

	XMFLOAT3 center;
	XMFLOAT3 orientation;
   	XMFLOAT3 size;
};

enum eModType { CUBE, CUBE_TEXTURED, PLANE, SPHERE, POLYGON, CUSTOM_MESH };

enum eTexture
{
	AsphaltFresh = 0,
	AsphaltTGA = 1,
	AsphaltOld = 2,
	Water = 3,
	SharkSkin = 4
};

////////////////////////////////////////////////////////////////
// FUNCTION FOR GETTING WORLD MATRIX
// TODO: make it global; see which is faster
////////////////////////////////////////////////////////////////
DirectX::XMMATRIX GetWorldMatrix( const ModelSpecs_W &modSpecs );

// Common vertex buffer types and corresponding input element descriptions

// Position and Color 
struct VertexPositionColorType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
	static std::vector<D3D11_INPUT_ELEMENT_DESC> CreateLayoutDescriptions();
};

// Position and texture coordinates
struct VertexPositionUVType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 uv;

	// Input layout descriptions for position and texture coordinates
	static std::vector<D3D11_INPUT_ELEMENT_DESC> CreateLayoutDescriptions();

};

// Position, normal and texture coordinates
struct VertexPositionUVNormalType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;

	// Input layout descriptions for position, normal and texture coordinates
	static std::vector<D3D11_INPUT_ELEMENT_DESC> CreateLayoutDescriptions();
};