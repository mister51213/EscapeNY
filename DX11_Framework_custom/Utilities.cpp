#include "Utilities.h"
#include <algorithm>

using PartFn = int( *)( XMFLOAT3*, const int, const int );

int PartitionY( XMFLOAT3* Array, const int Left, const int Right )
{
	const auto mid = Left + ( Right - Left ) / 2;
	const auto pivot = Array[ mid ].y;
	// move the mid point value to the front.
	std::swap( Array[ mid ], Array[ Left ] );
	int i = Left + 1;
	int j = Right;
	while ( i <= j )
	{
		while ( i <= j && Array[ i ].y <= pivot )
		{
			i++;
		}

		while ( i <= j && Array[ j ].y > pivot )
		{
			j--;
		}

		if ( i < j )
		{
			std::swap( Array[ i ], Array[ j ] );
		}
	}
	std::swap( Array[ i - 1 ], Array[ Left ] );
	return i - 1;
}
int PartitionZ( XMFLOAT3* Array, const int Left, const int Right )
{
	const auto mid = Left + ( Right - Left ) / 2;
	const auto pivot = Array[ mid ].z;
	// move the mid point value to the front.
	std::swap( Array[ mid ], Array[ Left ] );
	int i = Left + 1;
	int j = Right;
	while ( i <= j )
	{
		while ( i <= j && Array[ i ].z <= pivot )
		{
			i++;
		}

		while ( i <= j && Array[ j ].z > pivot )
		{
			j--;
		}

		if ( i < j )
		{
			std::swap( Array[ i ], Array[ j ] );
		}
	}
	std::swap( Array[ i - 1 ], Array[ Left ] );
	return i - 1;
}
int PartitionX( XMFLOAT3* Array, const int Left, const int Right )
{
	const auto mid = Left + ( Right - Left ) / 2;
	const auto pivot = Array[ mid ].x;
	// move the mid point value to the front.
	std::swap( Array[ mid ], Array[ Left ] );
	int i = Left + 1;
	int j = Right;
	while ( i <= j )
	{
		while ( i <= j && Array[ i ].x <= pivot )
		{
			i++;
		}

		while ( i <= j && Array[ j ].x > pivot )
		{
			j--;
		}

		if ( i < j )
		{
			std::swap( Array[ i ], Array[ j ] );
		}
	}
	std::swap( Array[ i - 1 ], Array[ Left ] );
	return i - 1;
}

void Quicksort( XMFLOAT3 *arr, const int left, const int right, PartFn Fn )
{
	if ( left >= right )
	{
		return;
	}

	int part = Fn( arr, left, right );

	Quicksort( arr, left, part - 1, Fn );
	Quicksort( arr, part + 1, right, Fn );
}

void Sort( std::vector<XMFLOAT3> &V )
{
	// Erase duplicate vertices from vertex list
	/*for ( int j = V.size() - 1; j >= 0; --j )
	{
		for ( int i = j - 1; i >= 0; --i )
		{
			if ( V[ i ] == V[ j ] )
			{
				V.erase( V.begin() + i );
			}
		}
	}*/

	// Sort by Y
	Quicksort( V.data(), 0, V.size() - 1, PartitionY );

	// Sort by Z
	float Y = V[ 0 ].y;
	int yStart = 0;	

	for ( int i = 0; i < V.size(); ++i )
	{
		if ( V[ i ].y != Y )
		{
			const int yEnd = i - 1;
			Quicksort( V.data(), yStart, yEnd, PartitionZ );
			yStart = i;
			Y = V[ yStart ].y;
		}
	}

	// Sort by X
	float Z = V[ 0 ].z;
	int zStart = 0;

	for ( int i = 0; i < V.size(); ++i )
	{
		if ( V[ i ].z != Z )
		{
			const int zEnd = i - 1;
			Quicksort( V.data(), zStart, zEnd, PartitionX );
			zStart = i;
			Z = V[ zStart ].z;
		}
	}
	int a = 0;
}

XMFLOAT2 operator+( const XMFLOAT2 & V1, const XMFLOAT2 & V2 )
{
	return XMFLOAT2(V1.x + V2.x, V1.y + V2.y);
}

XMFLOAT2 operator-( const XMFLOAT2 & V )
{
	return XMFLOAT2(-V.x, -V.y);
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
	return XMFLOAT3(-V.x, -V.y, -V.z);
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
	if( ( fabsf( X ) > 0.f ) || ( fabsf( Z ) > 0.f ) )
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

DirectX::XMMATRIX GetWorldMatrix( const ModelSpecs_W & modSpecs )
{
	auto xmPosition = XMLoadFloat3( &modSpecs.position );
	auto xmOrientation = XMLoadFloat3( &modSpecs.orientation );
	xmOrientation = ConvertToRadians( xmOrientation );
	auto xmScale = XMLoadFloat3( &modSpecs.scale );

	DirectX::XMMATRIX trans = XMMatrixTranslationFromVector( xmPosition );
	DirectX::XMMATRIX rot = 
		XMMatrixRotationRollPitchYawFromVector( xmOrientation );
	DirectX::XMMATRIX scal = XMMatrixScalingFromVector( xmScale );

	// Also transpose it in this step to make it easier for GPU to handle
	return XMMatrixTranspose( rot*scal*trans );
}

void Transpose( std::vector<XMMATRIX>& matrices )
{
	for( auto &matrix : matrices )
	{
		matrix = XMMatrixTranspose( matrix );
	}
}

ModelSpecs_L::ModelSpecs_L( const XMFLOAT3 & Center, const XMFLOAT3 & Orientation, const XMFLOAT3 & Size )
	:
	center( Center ),
	orientation( Orientation ),
	size( Size )
{
}

////////////////////////////////////////////////////
// TODO: Make generic function
// CreateCBufferDescription(UINT size)
// TODO: Make generic function CreateVertexBuffer(??)
/////////////////////////////////////////////////////
D3D11_BUFFER_DESC MatrixBufferType::CreateMatrixDescription()
{
    D3D11_BUFFER_DESC matrixBufferDesc{};
  	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
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
        { "POSITION", 0, float3Format, 0, 0, vertexDataType , 0},
        { "TEXCOORD", 0, float2Format, 0, appendAlignment, vertexDataType, 0},
        { "NORMAL", 0, float3Format, 0, appendAlignment, vertexDataType, 0 },
        { "COLOR", 0, float4Format, 0, appendAlignment, vertexDataType, 0 }
    };

    return desc;
}

D3D11_BUFFER_DESC LightBufferType::CreateLightDescription( unsigned int MaxLightCount )
{
    D3D11_BUFFER_DESC lightBufferDesc{};
    lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType) * MaxLightCount;
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
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
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

D3D11_BUFFER_DESC SceneBufferType::CreateLightDescription( )
{
	D3D11_BUFFER_DESC lightBufferDesc{};
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof( SceneBufferType );
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	return lightBufferDesc;
}
