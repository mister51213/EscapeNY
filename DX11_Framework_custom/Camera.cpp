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
    m_lookAtVector = XMVectorSet( 0.f, 0.f, 1.f, 0.f );
}

Camera::Camera(const Camera& other)
{
}

Camera::~Camera()
{
}

// Set up the position and rotation of the camera.
bool Camera::Initialize( 
    const XMFLOAT3 &Position, 
    const XMFLOAT3 &Rotation,
	const XMUINT2 &ScreenSize, 
    const XMFLOAT2 &ScreenClipDepth )
{
	SetPosition( Position );
	SetRotation( Rotation );
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
XMFLOAT3 Camera::GetPosition()const
{
	return m_Position;
}

XMFLOAT3 Camera::GetRotation()const
{
	return m_Rotation;
}

// Use the position and rotation of the camera to build and update the view matrix. 

void Camera::GetInput(std::shared_ptr<Input> pInput)
{
    float radius = pInput->GetRadius();
    float phi = pInput->GetPhi();
    float theta = pInput->GetTheta();
    // Convert Spherical to Cartesian coordinates.
    // NOTE: This info is passed on to the camera POSITION in ShapeBuilder.cpp line 171~
    float x = radius*sinf(phi)*cosf(theta);
    float z = radius*sinf(phi)*sinf(theta);
    float y = radius*cosf(phi);

    // TODO: this doesnt take target into account, is wrong!
    m_Position = { x,y,z };

    // TODO: need to use this for lookat:
    /* XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
        > THEN stores this view as a member variable m_view
        > THEN uses the m_view variable for rendering
    */
    // TODO: Integrate this into Game.cpp calculation of world, view, and projection matrixes
    /* 
     // Build the view matrix.
    XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
    XMStoreFloat4x4(&mView, view);

    XMMATRIX world = XMLoadFloat4x4(&mWorld);
    XMMATRIX proj = XMLoadFloat4x4(&mProj);
    XMMATRIX worldViewProj = world*view*proj;
    */

}

// called EVERY FRAME
void Camera::Render()
{
	// Load the rotation and make radian vectors.
	XMVECTOR rotationVector = XMLoadFloat3( &m_Rotation );

	// Setup the vector that points upwards.
	XMVECTOR upVector = XMVectorSet( 0.f, 1.f, 0.f, 0.f );

	// Load the position into an XMVECTOR structure.
	XMVECTOR positionVector = XMLoadFloat3(&m_Position);

    // TODO: MUST HAVE INPUT OPERATE ON THESE VALUES!!!!
	// Setup where the camera is looking by default.
	//XMVECTOR lookAtVector = XMVectorSet( 0.f, 0.f, 1.f, 0.f );
     m_lookAtVector = XMVectorSet( 0.f, 0.f, 1.f, 0.f );

	// Create the rotation matrix from the product of the rotation vector and the radian vector.
	// This converts the rotations to radians before creating the rotation matrix
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(ConvertToRadians(rotationVector));

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	//lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
    m_lookAtVector = XMVector3TransformCoord(m_lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	//lookAtVector = XMVectorAdd(positionVector, lookAtVector);
    m_lookAtVector = XMVectorAdd(positionVector, m_lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	//m_ViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
    m_ViewMatrix = XMMatrixLookAtLH(positionVector, m_lookAtVector, upVector);
}

///////////////////////////////////////////////////////////////////////////////
// After the Render function has been called to create the view matrix we can//
// provide the updated view matrix to calling functions using this 			 //
// GetViewMatrix function. The view matrix will be one of the three main 	 //
// matrices used in the HLSL vertex shader.									 //
///////////////////////////////////////////////////////////////////////////////
XMMATRIX Camera::GetViewMatrix()const
{
	return m_ViewMatrix;
}

// Gives access to projection, world, orthographic matrices because
// most shaders will need these matrices for rendering
XMMATRIX Camera::GetProjectionMatrix()const
{
	return m_ProjectionMatrix;
}

XMMATRIX Camera::GetOrthoMatrix()const
{
	return m_OrthoMatrix;
}