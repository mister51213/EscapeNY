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
    m_upV = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    m_positionV = XMVectorSet(0.f, 0.f, 0.f, 1.0f);
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
   /* m_radius = pInput->GetRadius();
    m_phi = pInput->GetPhi();
    m_theta = pInput->GetTheta();*/

    m_lookAtVector = XMLoadFloat3( &pInput->m_LastMousePos);

    // FPS motion implementation
    if (pInput->IsKeyDown('d'))
    {
        Move({ .6f, 0.f, 0.f });
    }
    if (pInput->IsKeyDown('a'))
    {
        Move({ -.6f, 0.f, 0.f });
    }
    if (pInput->IsKeyDown('w'))
    {
        Move({ 0.f, 0.f, .6f });
    }
    if (pInput->IsKeyDown('s'))
    {
        Move({ 0.f, 0.f, -.6f });
    }
}

// called EVERY FRAME - see ShapeBuilder.cpp line 171~
void Camera::Render()
{
    // must reset lookat vector every time
    XMVECTOR positionV = XMLoadFloat3(&m_Position);
    XMVECTOR fwdV = XMVectorSet (0.f, 0.f, 0.1f, 1.f);
    m_ViewMatrix = XMMatrixLookAtLH(positionV, m_lookAtVector, m_upV);
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