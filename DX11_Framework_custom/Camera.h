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
#include "Input.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Camera
////////////////////////////////////////////////////////////////////////////////
class Camera
{
public:
	// Default cause they don't do anything, so no need to define them
	Camera() = default;
	~Camera() = default;

	DirectX::XMMATRIX GetViewMatrix()const;
	DirectX::XMMATRIX GetProjectionMatrix()const;
	DirectX::XMMATRIX GetOrthoMatrix()const;
	DirectX::XMFLOAT3 GetPosition()const;
	DirectX::XMFLOAT3 GetRotation()const;
	void SetPosition( const DirectX::XMFLOAT3 &Position );
	void SetRotation( const DirectX::XMFLOAT3 &Rotation );

	bool Initialize(
		const DirectX::XMFLOAT3 &Position,
		const DirectX::XMFLOAT3 &Rotation,
		const DirectX::XMUINT2 &ScreenSize,
		const DirectX::XMFLOAT2 &ScreenClipDepth );

	void GetInput( const Input& pInput );

	void GetMouseInput( const Input& pInput );

	// ROTATE camera on the X axis
	void Pitch( float angle );

	// ROTATE camera on the Y axis
	void Yaw( float angle );

	// Translate camera position along the look vector
	void Walk( float distance );

	// Translate camera position along the look vector
	void FloatSink( float distance );

	// Translate camera position along the right vector
	void Strafe( float distance );

	void Render();

private:
	DirectX::XMFLOAT3 m_Position = { 0.0f, 500.0f, 0.0f };
	// How many degrees is it rotated on the x, y, z axes
	// JUST a collection of three different angles
	DirectX::XMFLOAT3 m_Orientation = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 m_RightDir = { 1.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 m_UpDir = { 0.0f, 1.0f, 0.0f };
	DirectX::XMFLOAT3 m_LookDir = { 0.0f, 0.0f, 1.0f };
	DirectX::XMMATRIX m_ViewMatrix, m_ProjectionMatrix, m_OrthoMatrix;;

	const float m_camSpeed = 20.f;
};