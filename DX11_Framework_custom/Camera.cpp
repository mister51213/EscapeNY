#include "Camera.h"

XMMATRIX Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

XMMATRIX Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

XMMATRIX Camera::GetOrthoMatrix() const
{
	return m_OrthoMatrix;
}

XMFLOAT3 Camera::GetPosition() const
{
	return m_Position;
}

XMFLOAT3 Camera::GetRotation() const
{
	return m_Orientation;
}

void Camera::SetPosition( const XMFLOAT3 & Position )
{
	m_Position = Position;
}

void Camera::SetRotation( const XMFLOAT3 & Rotation )
{
	m_Orientation = Rotation;
}

bool Camera::Initialize( 
	const XMFLOAT3 & Position, 
	const XMFLOAT3 & Rotation, 
	const XMUINT2 & ScreenSize, 
	const XMFLOAT2 & ScreenClipDepth )
{
	SetPosition( Position );
	SetRotation( Rotation );

	float screenWidth = static_cast<float>( ScreenSize.x );
	float screenHeight = static_cast<float>( ScreenSize.y );

	// Setup the projection matrix used to translate the 3D scene into the 2D viewport space
	float fieldOfView = XM_PIDIV4;
	float screenAspect = screenWidth / screenHeight;
	
	// Create the projection matrix for 3D rendering.
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(
		fieldOfView,
		screenAspect,
		ScreenClipDepth.x,
		ScreenClipDepth.y );
	
	// Create an orthographic projection matrix for 2D rendering.
	m_OrthoMatrix = XMMatrixOrthographicLH(
		screenWidth,
		screenHeight,
		ScreenClipDepth.x,
		ScreenClipDepth.y );
	
	return true;
}

void Camera::GetInput( const Input & pInput )
{
	const float time = 1.f;
	const float camSpeed = 2.f;
	float displacement = time*camSpeed;

	if( pInput.IsKeyDown( 'D' ) )
	{
		Strafe( displacement );
	}
	if( pInput.IsKeyDown( 'A' ) )
	{
		Strafe( -displacement );
	}
	if( pInput.IsKeyDown( 'W' ) )
	{
		Walk( displacement );
	}
	if( pInput.IsKeyDown( 'S' ) )
	{
		Walk( -displacement );
	}
	if( pInput.IsKeyDown( VK_PRIOR ) )
	{
		FloatSink( displacement );
	}
	if( pInput.IsKeyDown( VK_NEXT ) )
	{
		FloatSink( -displacement );
	}

}

void Camera::GetMouseInput( const Input & pInput )
{
	// Get "angle" traveled by mouse since last position
	float angleX = 0.25f*static_cast<float>( pInput.GetRelativeY() );
	float angleY = 0.25f*static_cast<float>( pInput.GetRelativeX() );

	// Re-rotate camera based on new position of mouse
	Pitch( angleX );
	Yaw( angleY );
}

void Camera::Pitch( float angle )
{
	m_Orientation.x += angle; // will be converted to radians in render()
	Clamp( m_Orientation.x, -80.f, 80.f );
}

void Camera::Yaw( float angle )
{
	m_Orientation.y -= angle; // will be converted to radians in render()
}

void Camera::Walk( float distance )
{
	// Load/Initialize the SIMD registers
	XMVECTOR distV = DirectX::XMVectorReplicate( distance );
	XMVECTOR lookV = XMLoadFloat3( &m_LookDir );
	XMVECTOR posV = XMLoadFloat3( &m_Position );
	XMVECTOR radianV = XMLoadFloat3( &m_Orientation );
	XMVECTOR mask = XMVectorSet( 1.f, 1.f, 1.f, .0f );

	// Convert angles in degrees to radians
	radianV = ConvertToRadians( radianV );

	// Load up rotation matrix w current orientation
	XMMATRIX rotMat = XMMatrixRotationRollPitchYawFromVector( radianV );

	// Transform look vector by rotation matrix
	XMVECTOR lookV_Rotated = XMVector3Transform( lookV, rotMat );

	// Store product of 1st 2 vectors + 3rd vector in m_Position
	// make sure that the Y component wont get incremented
	lookV_Rotated *= mask;
	posV = XMVectorMultiplyAdd( distV, lookV_Rotated, posV );

	// Store result back to position
	XMStoreFloat3( &m_Position, posV );
}

void Camera::FloatSink( float distance )
{
	// Load/Initialize the SIMD registers
	XMVECTOR distV = DirectX::XMVectorReplicate( distance );
	XMVECTOR riseV = XMLoadFloat3( &m_UpDir );
	XMVECTOR posV = XMLoadFloat3( &m_Position );
	XMVECTOR radianV = XMLoadFloat3( &m_Orientation );
	XMVECTOR mask = XMVectorSet( 1.f, 1.f, 1.f, .0f );

	// Convert angles in degrees to radians
	radianV = ConvertToRadians( radianV );

	// Load up rotation matrix w current orientation
	XMMATRIX rotMat = XMMatrixRotationRollPitchYawFromVector( radianV );

	// Transform look vector by rotation matrix
	XMVECTOR riseV_Rotated = XMVector3Transform( riseV, rotMat );

	// Store product of 1st 2 vectors + 3rd vector in m_Position
	// make sure that the Y component wont get incremented
	riseV_Rotated *= mask;
	posV = XMVectorMultiplyAdd( distV, riseV_Rotated, posV );

	// Store result back to position
	XMStoreFloat3( &m_Position, posV );
}

void Camera::Strafe( float distance )
{
	// Load/Initialize the SIMD registers
	// create a vector to represent distance traveled
	XMVECTOR distV = DirectX::XMVectorReplicate( distance );
	XMVECTOR rightV = XMLoadFloat3( &m_RightDir );
	XMVECTOR posV = XMLoadFloat3( &m_Position );
	XMVECTOR radianV = XMLoadFloat3( &m_Orientation );
	XMVECTOR mask = XMVectorSet( 1.f, 1.f, 1.f, .0f );

	// Convert angles in degrees to radians
	radianV = ConvertToRadians( radianV );

	// Load up rotation matrix w current orientation
	XMMATRIX rotMat = XMMatrixRotationRollPitchYawFromVector( radianV );

	// Transform look vector by rotation matrix
	XMVECTOR rightV_Rotated = XMVector3Transform( rightV, rotMat );

	// Store product of 1st 2 vectors + 3rd vector in m_Position
	posV = XMVectorMultiplyAdd( distV, rightV_Rotated, posV );
	XMStoreFloat3( &m_Position, posV );
}

void Camera::Render()
{
	// Load/Initialize SIMD registers
	XMVECTOR posV = XMLoadFloat3( &m_Position );
	XMVECTOR rotV = XMLoadFloat3( &m_Orientation );

	// Convert rotV vector to radians
	rotV = ConvertToRadians( rotV );

	// Build view matrix from position and rotation
	XMMATRIX translationMat = XMMatrixTranslationFromVector( posV );
	XMMATRIX rotationMat = XMMatrixRotationRollPitchYawFromVector( rotV );

	// ORDER of multiplication matters here:
	m_ViewMatrix = XMMatrixInverse( 0, rotationMat*translationMat );
}