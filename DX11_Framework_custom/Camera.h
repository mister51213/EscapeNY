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
#include "Input.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: Camera
////////////////////////////////////////////////////////////////////////////////
class Camera
{
public:
	Camera();
	~Camera();
    // WORLD matrix can be decomposed into a rotation followed by a translation.
    // W = R T, View = inverse of W

void GetInput(std::shared_ptr<Input> pInput)
{
    const float time = 1.f;
    const float camSpeed = 2.f;
    float displacement = time*camSpeed;

    if (pInput->IsKeyDown('d'))
    {
        Strafe(displacement);
    }
    if (pInput->IsKeyDown('a'))
    {
        Strafe(-displacement);
    }
    if (pInput->IsKeyDown('w'))
    {
        Walk(displacement);
    }
    if (pInput->IsKeyDown('s'))
    {
        Walk(-displacement);
    }
} 

    // ROTATE camera on the X axis
    void Pitch(float angle)
    {
    XMMATRIX RotationM = XMMatrixRotationAxis(XMLoadFloat3(&m_RightDir), angle);
	XMStoreFloat3(&m_UpDir,   XMVector3TransformNormal(XMLoadFloat3(&m_UpDir), RotationM));
	XMStoreFloat3(&m_LookDir, XMVector3TransformNormal(XMLoadFloat3(&m_LookDir), RotationM));
    }

    // ROTATE camera on the Y axis
    void Yaw(float angle)
    {    
    XMFLOAT3 upReset { 0.f,1.f,0.f }; // Reset up vector to avoid tilt
    XMMATRIX RotationM = XMMatrixRotationAxis(XMLoadFloat3(&upReset), angle);
	XMStoreFloat3(&m_RightDir,   XMVector3TransformNormal(XMLoadFloat3(&m_RightDir), RotationM));
	XMStoreFloat3(&m_LookDir, XMVector3TransformNormal(XMLoadFloat3(&m_LookDir), RotationM));
    }

    // Translate camera position along the look vector
    void Walk(float distance)
    {
    XMVECTOR distV = DirectX::XMVectorReplicate(distance);
    XMVECTOR lookV = XMLoadFloat3(&m_LookDir);
	XMVECTOR posV = XMLoadFloat3(&m_Position);
    // Store product of 1st 2 vectors + 3rd vector in m_Position
	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(distV, lookV, posV));
    }

    // Translate camera position along the right vector
    void Strafe(float distance)
    {
	XMVECTOR distV = DirectX::XMVectorReplicate(distance);
    XMVECTOR rightV = XMLoadFloat3(&m_RightDir);
	XMVECTOR posV = XMLoadFloat3(&m_Position);
    // Store product of 1st 2 vectors + 3rd vector in m_Position
	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(distV, rightV, posV));
    }

    void Render()
    {


        m_viewMatrix = XMMatrixIdentity();
    }

private:
    XMFLOAT3 m_Position = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 m_Rotation = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 m_RightDir = { 1.0f, 0.0f, 0.0f };
    XMFLOAT3 m_UpDir = { 0.0f, 1.0f, 0.0f };
    XMFLOAT3 m_LookDir = { 0.0f, 0.0f, 1.0f };
    XMMATRIX m_viewMatrix;
};