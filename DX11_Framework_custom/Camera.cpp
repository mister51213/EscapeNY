/*****************************************************************************************
 *This camera class will to let DirectX 11 know from where and also how we are viewing   *
 *the scene. The camera class will keep track of where the camera is and its current     *
 *rotation. It will use the position and rotation information to generate a view matrix  *
 *which will be passed into the HLSL shader for rendering.                               *
 *                                                                                       *
 *****************************************************************************************/
#include "Camera.h"

// The constructor will initialize the position and rotation of the camera to be at the 
// origin of the scene.

Camera::Camera()
	:
	m_Position( 0.f, 0.f, 0.f ),
	m_Rotation( 0.f, 0.f, 0.f )
{
}

Camera::Camera(const Camera& other)
{
}

Camera::~Camera()
{
}

// Set up the position and rotation of the camera.
bool Camera::Initialize( 
	const XMUINT2 &ScreenSize, 
    const XMFLOAT2 &ScreenClipDepth )
{
	float screenWidth = static_cast<float>( ScreenSize.x );
	float screenHeight = static_cast<float>( ScreenSize.y );

	// Setup the projection matrix used to translate the 3D scene 
	// into the 2D viewport space
	float fieldOfView = XM_PIDIV4;
	float screenAspect = screenWidth / screenHeight;

	// Create the projection matrix for 3D rendering.
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH( 
        fieldOfView, 
        screenAspect, 
        ScreenClipDepth.x, 
        ScreenClipDepth.y );

	// Create an orthographic projection matrix for 2D rendering.
	// used for rendering 2D elements like user interfaces on the screen 
	// allowing us to skip the 3D rendering.
	m_OrthoMatrix = XMMatrixOrthographicLH( 
        screenWidth, 
        screenHeight, 
        ScreenClipDepth.x, 
        ScreenClipDepth.y );
	
	return true;
}

void Camera::SetPosition( const XMFLOAT3 &Position )
{
	m_Position = Position;
}

void Camera::SetRotation( const XMFLOAT3 &Rotation )
{
	m_Rotation = Rotation;
}

// return the location and rotation of the camera to calling functions.
const XMFLOAT3 &Camera::GetPosition()const
{
	return m_Position;
}

const XMFLOAT3 &Camera::GetRotation()const
{
	return m_Rotation;
}

void Camera::Move( XMFLOAT3 offset )
{
	m_Position.x += offset.x;
	m_Position.y += offset.y;
	m_Position.z += offset.z;
}

void Camera::Rotate( XMFLOAT3 offset )
{
	m_Rotation.x += offset.x;
	m_Rotation.y += offset.y;
	m_Rotation.z += offset.z;
}

// Use the position and rotation of the camera to build and update the view matrix. 

void Camera::Update( const XMFLOAT3 &LookAtV )
{
	// Load the rotation and make radian vectors.
	XMVECTOR rotationVector = XMLoadFloat3( &m_Rotation );

	// Setup the vector that points upwards.
	XMVECTOR upVector = XMVectorSet( 0.f, 1.f, 0.f, 0.f );

	// Load the position into an XMVECTOR structure.
	XMVECTOR positionVector = XMLoadFloat3(&m_Position);

	// Setup where the camera is looking by default.
	XMVECTOR lookAtVector = XMVectorSet( 0.f, 0.f, 1.f, 0.f );
	//XMVECTOR lookAtVector = XMLoadFloat3( &LookAtV );

	// Create the rotation matrix from the product of the rotation vector and the radian vector.
	// This converts the rotations to radians before creating the rotation matrix
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(ConvertToRadians(rotationVector));

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_ViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

///////////////////////////////////////////////////////////////////////////////
// After the Update function has been called to create the view matrix we can//
// provide the updated view matrix to calling functions using this 			 //
// GetViewMatrix function. The view matrix will be one of the three main 	 //
// matrices used in the HLSL vertex shader.									 //
///////////////////////////////////////////////////////////////////////////////
const XMMATRIX &Camera::GetViewMatrix()const
{
	return m_ViewMatrix;
}

const XMMATRIX &Camera::GetProjectionMatrix()const
{
	return m_ProjectionMatrix;
}

const XMMATRIX &Camera::GetOrthoMatrix()const
{
	return m_OrthoMatrix;
}