#include "MathUtils.h"

XMFLOAT2 operator+( const XMFLOAT2 & V1, const XMFLOAT2 & V2 )
{
	return XMFLOAT2( V1.x + V2.x, V1.y + V2.y );
}

XMFLOAT2 operator-( const XMFLOAT2 & V )
{
	return XMFLOAT2( -V.x, -V.y );
}

XMFLOAT2 operator-( const XMFLOAT2 & V1, const XMFLOAT2 & V2 )
{
	return XMFLOAT2( V1.x - V2.x, V1.y - V2.y );
}

XMFLOAT2 operator*( const XMFLOAT2 & V, const float S )
{
	return{ V.x * S, V.y * S };
}

XMFLOAT3 operator+( const XMFLOAT3 & V1, const XMFLOAT3 & V2 )
{
	return XMFLOAT3( V1.x + V2.x, V1.y + V2.y, V1.z + V2.z );
}

XMFLOAT3 operator-( const XMFLOAT3 & V )
{
	return XMFLOAT3( -V.x, -V.y, -V.z );
}

XMFLOAT3 operator-( const XMFLOAT3 & V1, const XMFLOAT3 & V2 )
{
	return XMFLOAT3( V1.x - V2.x, V1.y - V2.y, V1.z - V2.z );
}

XMFLOAT3 operator*( const XMFLOAT3 & V, const float S )
{
	return XMFLOAT3( V.x * S, V.y * S, V.z * S );
}

XMFLOAT3 operator/( const XMFLOAT3 & V, const float S )
{
	const float recipScalar = 1.f / S;
	return XMFLOAT3( V.x * recipScalar, V.y * recipScalar, V.z * recipScalar );
}

XMFLOAT3 & operator+=( XMFLOAT3 & V1, const XMFLOAT3 & V2 )
{
	V1 = V2 + V1;
	return V1;
}

XMFLOAT3 & operator-=( XMFLOAT3 & V1, const XMFLOAT3 & V2 )
{
	V1 = V2 - V1;
	return V1;
}

XMFLOAT3 & operator*=( XMFLOAT3 & V, const float S )
{
	V = V * S;
	return V;
}

XMFLOAT3 & operator/=( XMFLOAT3 & V, const float S )
{
	V = V / S;
	return V;
}

bool operator==( const XMFLOAT3 & A, const XMFLOAT3 & B )
{
	return A.x == B.x && A.y == B.y && A.z == B.z;
}

XMFLOAT3 CrossProduct( const XMFLOAT3 & V1, const XMFLOAT3 & V2 )
{
	return
	{
		( V1.y * V2.z ) - ( V1.z * V2.y ),
		( V1.z * V2.x ) - ( V1.x * V2.z ),
		( V1.x * V2.y ) - ( V1.y * V2.x )
	};
}

float DotProduct( const XMFLOAT2 & V1, const XMFLOAT2 & V2 )
{
	return ( ( V1.x * V2.x ) + ( V1.y * V2.y ) );
}

float DotProduct( const XMFLOAT3 & V1, const XMFLOAT3 & V2 )
{
	return ( V1.x * V2.x ) + ( V1.y * V2.y ) + ( V1.z * V2.z );
}

float Magnitude( const XMFLOAT2 & V )
{
	return sqrt( DotProduct( V, V ) );
}

float Magnitude( const XMFLOAT3 & V )
{
	// The dot-product of an angle with itself is the same as
	// the magnitude of the vector squared, to get the magnitude
	// return the square root of the result.
	return sqrtf( DotProduct( V, V ) );
}

float Length( const XMFLOAT2 & V1, const XMFLOAT2 & V2 )
{
	return Magnitude( V2 - V1 );
}

float Length( const XMFLOAT3 & V1, const XMFLOAT3 & V2 )
{
	// Length between two points is the magnitude of the vector
	// that starts at point1 and goes to point2
	return Magnitude( V2 - V1 );
}

XMFLOAT2 Normalize( const XMFLOAT2 & V )
{
	return V * ( 1.f / Magnitude( V ) );
}

XMFLOAT3 Normalize( const XMFLOAT3 & V )
{
	// Optimization
	// Division is the slowest basic math operation, so getting
	// the recipricol we can use multiplication instead, which is faster
	float recipricalLength = 1.0f / Magnitude( V );
	return V * recipricalLength;
}

XMVECTOR ConvertToRadians( const XMVECTOR & angleInDegrees )
{
	// Use constexpr radian which is the result of PI / 180.f calculated
	// at compile time.
	return angleInDegrees * XMVectorReplicate( g_radian );
}

XMFLOAT3 ConvertToRadians( const XMFLOAT3 & angleInDegrees )
{
	// Use constexpr radian which is the result of PI / 180.f calculated
	// at compile time.
	return{ angleInDegrees * g_radian };
}

float CalculateYRotation( const float X, const float Z, float & RX, float & RZ )
{
	// If x or z is greater than 0.f, calculate new direction, otherwise
	// skips calculations, uses trig function acos (arccosine) to convert
	// vector direction to angle in radians, then to degrees.
	float angle = -0.f;
	if ( ( fabsf( X ) > 0.f ) || ( fabsf( Z ) > 0.f ) )
	{
		// Normalize X and Z to get direction
		// Get the recipricol magnitude of the 2D vector
		float recipricolMagnitude = 1.f / sqrtf( pow( X, 2 ) + pow( Z, 2 ) );

		// Use the recipricol magnitude to get the normalized 2D vector.
		// Using the recipricol of the magnitude is faster, because multiplying
		// is faster than dividing
		RX = X * recipricolMagnitude;
		RZ = Z * recipricolMagnitude;

		// Get the angle in radians, depending on which you use, result
		// could be positive or negative
		float radx = acosf( RX );
		float radz = asinf( RZ );

		// Determine the sign (positive or negative) of the angle
		// If both are positive, angle will be positive,
		// if one is negative or both, then angle will be negative
		float sgn = ( radx > 0.f ) && ( radz > 0.f ) ? 1.f : -1.f;

		// Convert to degrees
		angle = g_degree * ( sgn * radx );
	}

	return angle;
}

