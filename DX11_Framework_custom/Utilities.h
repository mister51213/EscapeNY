#pragma once

#include "Includes.h"

using namespace std;

// Permit me to be a little lazy :)
// This is just an alias to shorten the amount of typing
template<class T>
using comptr = Microsoft::WRL::ComPtr<T>;

#pragma warning(disable:4067)
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
// Algorithms
////////////////////////////////////////////////////

void Sort( std::vector<DirectX::XMFLOAT3> &V );

//////////////////////////
/// GAME LOGIC RELATED ///
//////////////////////////
enum eState
{
	Stationary, Falling, Moving, Move_PID
};

////////////////////////
/// PHYSICS RELATED  ///
////////////////////////
// "downward" acceleration on the Y axis
const float Gravity = -9.8f; // units are meters/second^2

struct PhysAttributes
{
	// would need to calculate this for more realistic physics.
	float terminalV = 50.0f;
	DirectX::XMFLOAT3 velocity = { 0.f, 0.0f, 0.f };
	DirectX::XMFLOAT3 posError = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 target = { 0.0f, 0.0f, 0.0f };
};

enum eForceType
{
	Push,
	Pull,
	Spin,
	Swerve
};

struct BoundingBox
{
	vector<XMFLOAT3> vertices;
};

//////////////////////
// GRAPHICS RELATED //
//////////////////////
/////////////////////////////
// CLAMP FUNCTION
/////////////////////////////
template<typename T>
static T Clamp( const T& x, const T& low, const T& high )
{
	return x < low ? low : ( x > high ? high : x );
}

////////////////////////////////////////////////////////////////
// STRUCTS CONTAINING MODEL INFO for gameplay and rendering
// TODO: make it global; see which is faster
////////////////////////////////////////////////////////////////
struct ModelSpecs_W
{
	DirectX::XMFLOAT3 position, orientation, scale;
};

struct ModelSpecs_L
{
	ModelSpecs_L(
		const DirectX::XMFLOAT3 &Center = { 0.f, 0.f, 0.f },
		const DirectX::XMFLOAT3 &Orientation = { 0.f, 0.f, 0.f },
		const DirectX::XMFLOAT3 &Size = { .5f, .5f, .5f } );

	DirectX::XMFLOAT3 center;
	DirectX::XMFLOAT3 orientation;
	DirectX::XMFLOAT3 size;
};

enum eModType
{
	CUBE,
	CUBE_TEXTURED,
	PLANE,
	SPHERE,
	POLYGON,
	CUSTOM_MESH,
	CUSTOM_MESH2,
	CUSTOM_MESH3,
	CUSTOM_MESH4,
	SOME_EDIFACE,
	CAR
};

enum eTexture
{
	AsphaltFresh = 0,
	AsphaltTGA = 1,
	AsphaltOld = 2,
	Water1 = 3,
	Water2 = 4,
	Water3 = 5,
	Underwater1 = 6,
	Underwater2 = 7,
	Underwater3 = 8,
	Underwater4 = 9,
	Underwater5 = 10,
	Underwater6 = 11,
	Underwater7 = 12,
	SharkSkin = 13,
	Aluminum = 14,
	Lava = 15,
	Lava2 = 16
};

////////////////////////////////////////////////////////////////
// FUNCTION FOR GETTING WORLD MATRIX
// TODO: make it global; see which is faster
////////////////////////////////////////////////////////////////
DirectX::XMMATRIX GetWorldMatrix( const ModelSpecs_W &modSpecs );

