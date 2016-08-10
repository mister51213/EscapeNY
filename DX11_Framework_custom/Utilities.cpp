#include "Utilities.h"

XMFLOAT3 operator+( const XMFLOAT3 & V1, const XMFLOAT3 & V2 )
{
	return XMFLOAT3( V1.x + V2.x, V1.y + V2.y, V1.z + V2.z );
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

XMFLOAT3 CrossProduct( const XMFLOAT3 & V1, const XMFLOAT3 & V2 )
{
	return
	{
		( V1.y * V2.z ) - ( V1.z * V2.y ),
		( V1.z * V2.x ) - ( V1.x * V2.z ),
		( V1.x * V2.y ) - ( V1.y * V2.x )
	};
}

float DotProduct( const XMFLOAT3 & V1, const XMFLOAT3 & V2 )
{
	return ( V1.x * V2.x ) + ( V1.y * V2.y ) + ( V1.z * V2.z );
}

float Magnitude( const XMFLOAT3 & V )
{
	// The dot-product of an angle with itself is the same as
	// the magnitude of the vector squared, to get the magnitude
	// return the square root of the result.
	return sqrtf( DotProduct( V, V ) );
}

float Length( const XMFLOAT3 & V1, const XMFLOAT3 & V2 )
{
	// Length between two points is the magnitude of the vector
	// that starts at point1 and goes to point2
	return Magnitude( V2 - V1 );
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
	return angleInDegrees * XMVectorReplicate( radian );
}

XMFLOAT3 ConvertToRadians( const XMFLOAT3 & angleInDegrees )
{
	// Use constexpr radian which is the result of PI / 180.f calculated
	// at compile time.
	return{ angleInDegrees * radian };
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
		angle = degree * ( sgn * radx );
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

ModelSpecs_L::ModelSpecs_L( const XMFLOAT3 & Center, const XMFLOAT3 & Orientation, const XMFLOAT3 & Size )
	:
	center( Center ),
	orientation( Orientation ),
	size( Size )
{
}

std::vector<D3D11_INPUT_ELEMENT_DESC> VertexPositionColorType::CreateLayoutDescriptions()
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

std::vector<D3D11_INPUT_ELEMENT_DESC> VertexPositionUVType::CreateLayoutDescriptions()
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

std::vector<D3D11_INPUT_ELEMENT_DESC> VertexPositionUVNormalType::CreateLayoutDescriptions()
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

	eDesc[ 2 ].SemanticName = "NORMAL";
	eDesc[ 2 ].SemanticIndex = 0;
	eDesc[ 2 ].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	eDesc[ 2 ].InputSlot = 0;
	eDesc[ 2 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	eDesc[ 2 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	eDesc[ 2 ].InstanceDataStepRate = 0;

	return eDesc;
}
