#pragma once

#include "Includes.h"

using namespace DirectX;

////////////////////////////////////////////////////
// Vector Math Operator Overloads
///////////////////////////////////////////////////
// Add more when/if needed
XMFLOAT2 operator+( const XMFLOAT2 &V1, const XMFLOAT2 &V2 );
XMFLOAT2 operator-( const XMFLOAT2 &V );
XMFLOAT2 operator-( const XMFLOAT2 &V1, const XMFLOAT2 &V2 );
XMFLOAT2 operator*( const XMFLOAT2 &V, const float S );

// Keep operator overloads out of namespace so we can access them
// without having to declare: using namespace
XMFLOAT3 operator+( const XMFLOAT3 &V1, const XMFLOAT3& V2 );
XMFLOAT3 operator-( const XMFLOAT3 &V1, const XMFLOAT3& V2 );
XMFLOAT3 operator-( const XMFLOAT3 &V );
XMFLOAT3 operator*( const XMFLOAT3 &V, const float S );
XMFLOAT3 operator/( const XMFLOAT3 &V, const float S );
XMFLOAT3 &operator+= ( XMFLOAT3& V1, const XMFLOAT3& V2 );
XMFLOAT3 &operator-= ( XMFLOAT3& V1, const XMFLOAT3& V2 );
XMFLOAT3 &operator*= ( XMFLOAT3 &V, const float S );
XMFLOAT3 &operator/= ( XMFLOAT3 &V, const float S );

bool operator==( const XMFLOAT3 &A, const XMFLOAT3 &B );
bool operator!=( const XMFLOAT3 &A, const XMFLOAT3 &B );

// PI is equal to 180 degrees
constexpr float g_PI = 3.141592654f;

// TwoPI is equal to 360 degrees
constexpr float g_TwoPI = 2.f * g_PI;

// PIDivFour is equal to 45 degrees
constexpr float g_PIDivFour = g_PI / 4.f;

// PIDivTwo is equal to 90 degrees
constexpr float g_PIDivTwo = g_PI / 2.f;

// Radian is 0.01745329238f degrees, multiply angle in degrees by radian 
// to convert degrees to radians for rotation functions
constexpr float g_radian = g_PI / 180.f;

// Degree is 1 radian, 180 / PI, multiply angle in radians by degree to 
// convert radians to degrees
constexpr float g_degree = 180.f / g_PI;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sin values for theta = 90, 75, 60, 45, 30, 15, 0
constexpr float sinValues[] =
{
	1.0f,
	0.9659258f,
	0.8660254f,
	0.707106f,
	0.5f,
	0.258819f,
	0.0f
};

// cos values for theta = 90, 75, 60, 45, 30, 15, 0
constexpr float cosValues[ 7 ] =
{
	0.0f,
	0.258819f,
	0.5f,
	0.707106f,
	0.8660254f,
	0.9659258f,
	1.0f
};

constexpr float sqSinValues[] =
{
	sinValues[ 0 ] * sinValues[ 0 ],
	sinValues[ 1 ] * sinValues[ 1 ],
	sinValues[ 2 ] * sinValues[ 2 ],
	sinValues[ 3 ] * sinValues[ 3 ],
	sinValues[ 4 ] * sinValues[ 4 ],
	sinValues[ 5 ] * sinValues[ 5 ],
	sinValues[ 6 ] * sinValues[ 6 ],
};

constexpr float sqCosValues[] =
{
	cosValues[ 0 ] * cosValues[ 0 ],
	cosValues[ 1 ] * cosValues[ 1 ],
	cosValues[ 2 ] * cosValues[ 2 ],
	cosValues[ 3 ] * cosValues[ 3 ],
	cosValues[ 4 ] * cosValues[ 4 ],
	cosValues[ 5 ] * cosValues[ 5 ],
	cosValues[ 6 ] * cosValues[ 6 ],
};


// Calculates the cross-product of two float3 vectors
XMFLOAT3 CrossProduct( const XMFLOAT3 &V1, const XMFLOAT3 &V2 );

// Calculates the dot-product of two vectors
float DotProduct( const XMFLOAT2 &V1, const XMFLOAT2 &V2 );
float DotProduct( const XMFLOAT3 &V1, const XMFLOAT3 &V2 );

// Calculates the magnitude of a vector
float Magnitude( const XMFLOAT2 &V );
float Magnitude( const XMFLOAT3 &V );

// Calcualtes the length between two vectors
float Length( const XMFLOAT2 &V1, const XMFLOAT2 &V2 );
float Length( const XMFLOAT3 &V1, const XMFLOAT3 &V2 );

// Normalizes a vector
XMFLOAT2 Normalize( const XMFLOAT2 &V );
XMFLOAT3 Normalize( const XMFLOAT3 &V );

XMVECTOR ConvertToRadians( const XMVECTOR& angleInDegrees );

XMFLOAT3 ConvertToRadians( const XMFLOAT3& angleInDegrees );

template<class MathType>
MathType Square( const MathType &Val )
{
	return Val * Val;
}