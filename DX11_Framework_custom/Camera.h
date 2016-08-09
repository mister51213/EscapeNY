/*****************************************************************************************
 *This camera class will to let DirectX 11 know from where and also how we are viewing   *
 *the scene. The camera class will keep track of where the camera is and its current     *
 *rotation. It will use the position and rotation information to generate a view matrix  *
 *which will be passed into the HLSL shader for rendering.                               *
 *                                                                                       *
 *****************************************************************************************/
#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: camera.h
////////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "Includes.h"
#include "Utilities.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: Camera
////////////////////////////////////////////////////////////////////////////////
class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	// Initialize the camera object
	bool Initialize( const XMUINT2 &ScreenSize, const XMFLOAT2 &ScreenClipDepth );

    // set the position and rotation of the camera object. 
	void SetPosition( const XMFLOAT3 &Position );
	void SetRotation( const XMFLOAT3 &Rotation );

	const XMFLOAT3 &GetPosition()const;
	const XMFLOAT3 &GetRotation()const;

	void Move( XMFLOAT3 offset );
	void Rotate( XMFLOAT3 offset );

    // create the view matrix based on the position and rotation of the camera. 
	void Update();

    // retrieve the view matrix from the camera object so that the 
    // shaders can use it for rendering.
	const XMMATRIX &GetViewMatrix()const;
	const XMMATRIX &GetProjectionMatrix()const;
	const XMMATRIX &GetOrthoMatrix()const;

private:
	XMFLOAT3 m_Position, m_Rotation;
	XMMATRIX m_ViewMatrix, m_ProjectionMatrix, m_OrthoMatrix;
};