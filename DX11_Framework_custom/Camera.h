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
    Camera() {}
    ~Camera() {}

#pragma region ACCESSORS
    XMMATRIX GetViewMatrix()const
    {
        return m_ViewMatrix;
    }
    XMMATRIX GetProjectionMatrix()const
    {
        return m_ProjectionMatrix;
    }
    XMMATRIX GetOrthoMatrix()const
    {
        return m_OrthoMatrix;
    }
    XMFLOAT3 GetPosition()const
    {
        return m_Position;
    }
    XMFLOAT3 GetRotation()const
    {
        return m_Orientation;
    }
    void SetPosition(const XMFLOAT3 &Position)
    {
        m_Position = Position;
    }
    void SetRotation(const XMFLOAT3 &Rotation)
    {
        m_Orientation = Rotation;
    }
#pragma endregion

    bool Initialize(
        const XMFLOAT3 &Position,
        const XMFLOAT3 &Rotation,
        const XMUINT2 &ScreenSize,
        const XMFLOAT2 &ScreenClipDepth) {
        SetPosition(Position);
        SetRotation(Rotation);
        float screenWidth = static_cast<float>(ScreenSize.x);
        float screenHeight = static_cast<float>(ScreenSize.y);
        // Setup the projection matrix used to translate the 3D scene into the 2D viewport space
        float fieldOfView = XM_PIDIV4;
        float screenAspect = screenWidth / screenHeight;
        // Create the projection matrix for 3D rendering.
        m_ProjectionMatrix = XMMatrixPerspectiveFovLH(
            fieldOfView,
            screenAspect,
            ScreenClipDepth.x,
            ScreenClipDepth.y);
        // Create an orthographic projection matrix for 2D rendering.
        m_OrthoMatrix = XMMatrixOrthographicLH(
            screenWidth,
            screenHeight,
            ScreenClipDepth.x,
            ScreenClipDepth.y);
        return true;
    }

    void GetInput(const Input& pInput)
    {
        const float time = 1.f;
        const float camSpeed = 2.f;
        float displacement = time*camSpeed;

        if (pInput.IsKeyDown('D'))
        {
            Strafe(displacement);
        }
        if (pInput.IsKeyDown('A'))
        {
            Strafe(-displacement);
        }
        if (pInput.IsKeyDown('W'))
        {
            Walk(displacement);
        }
        if (pInput.IsKeyDown('S'))
        {
            Walk(-displacement);
        }
        if (pInput.IsKeyDown(VK_PRIOR))
        {
            FloatSink(displacement);
        }
        if (pInput.IsKeyDown(VK_NEXT))
        {
            FloatSink(-displacement);
        }

    }

    void GetMouseInput(const Input& pInput)
    {
    // Get "angle" traveled by mouse since last position
    float angleX = 0.25f*static_cast<float>(pInput.m_LastMousePos.y - m_LastMousePos.y);
    float angleY = 0.25f*static_cast<float>(pInput.m_LastMousePos.x - m_LastMousePos.x);

    // Store current mouse position
    m_LastMousePos.x = pInput.m_LastMousePos.x;
    m_LastMousePos.y = pInput.m_LastMousePos.y;

    // Re-rotate camera based on new position of mouse
    Pitch(angleX);
    Yaw(angleY);
    }

    // ROTATE camera on the X axis
    void Pitch(float angle)
    {
        m_Orientation.x += angle; // will be converted to radians in render()
        Clamp(m_Orientation.x, -80.f, 80.f);
    }

    // ROTATE camera on the Y axis
    void Yaw(float angle)
    {
        m_Orientation.y -= angle; // will be converted to radians in render()
    }

    // Translate camera position along the look vector
    void Walk(float distance)
    {
        XMVECTOR distV = DirectX::XMVectorReplicate(distance);
        XMVECTOR lookV = XMLoadFloat3(&m_LookDir);
        // Load up rotation matrix w current orientation
        XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(
            XMConvertToRadians(m_Orientation.x), 
            XMConvertToRadians(m_Orientation.y), 
            XMConvertToRadians(m_Orientation.z));
        // Transform look vector by rotation matrix
        XMVECTOR lookV_Rotated = XMVector3Transform(lookV, rotMat);

        XMVECTOR posV = XMLoadFloat3(&m_Position);
        // Store product of 1st 2 vectors + 3rd vector in m_Position
        // make sure that the Y component wont get incremented
        lookV_Rotated *= XMVectorSet(1.f, 1.f, 1.f, .0f);
        XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(distV, lookV_Rotated, posV));
    }
        // Translate camera position along the look vector
    void FloatSink(float distance)
    {
        XMVECTOR distV = DirectX::XMVectorReplicate(distance);
        XMVECTOR riseV = XMLoadFloat3(&m_UpDir);
        // Load up rotation matrix w current orientation
        XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(
            XMConvertToRadians(m_Orientation.x), 
            XMConvertToRadians(m_Orientation.y), 
            XMConvertToRadians(m_Orientation.z));
        // Transform look vector by rotation matrix
        XMVECTOR riseV_Rotated = XMVector3Transform(riseV, rotMat);

        XMVECTOR posV = XMLoadFloat3(&m_Position);
        // Store product of 1st 2 vectors + 3rd vector in m_Position
        // make sure that the Y component wont get incremented
        riseV_Rotated *= XMVectorSet(1.f, 1.f, 1.f, .0f);
        XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(distV, riseV_Rotated, posV));
    }

    // Translate camera position along the right vector
    void Strafe(float distance)
    {
        // create a vector to represent distance traveled
        XMVECTOR distV = DirectX::XMVectorReplicate(distance);

        XMVECTOR rightV = XMLoadFloat3(&m_RightDir);
        // Load up rotation matrix w current orientation
        XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(
                XMConvertToRadians(m_Orientation.x), 
                XMConvertToRadians(m_Orientation.y), 
                XMConvertToRadians(m_Orientation.z));
        // Transform look vector by rotation matrix
        XMVECTOR rightV_Rotated = XMVector3Transform(rightV, rotMat);

        XMVECTOR posV = XMLoadFloat3(&m_Position);
        // Store product of 1st 2 vectors + 3rd vector in m_Position
        XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(distV, rightV_Rotated, posV));
    }

    void Render()
    {
        // Build view matrix from position and rotation
        XMMATRIX translationMat = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
        XMMATRIX rotationMat = XMMatrixRotationRollPitchYaw(
            XMConvertToRadians(m_Orientation.x), 
            XMConvertToRadians(m_Orientation.y), 
            XMConvertToRadians(m_Orientation.z));
        // ORDER of multiplication matters here:
        m_ViewMatrix = XMMatrixInverse(0, rotationMat*translationMat);
    }

private:
    XMFLOAT3 m_Position = { 0.0f, 200.0f, 0.0f };
    // How many degrees is it rotated on the x, y, z axes
    // JUST a collection of three different angles
    XMFLOAT3 m_Orientation = { 0.0f, 0.0f, 0.0f };
    //XMFLOAT3 m_Rotation = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 m_RightDir = { 1.0f, 0.0f, 0.0f };
    XMFLOAT3 m_UpDir = { 0.0f, 1.0f, 0.0f };
    XMFLOAT3 m_LookDir = { 0.0f, 0.0f, 1.0f };
    XMMATRIX m_ViewMatrix, m_ProjectionMatrix, m_OrthoMatrix;;
    XMFLOAT3 m_LastMousePos = { 0.f,0.f,0.f };

    //float m_Pitch, m_Yaw;
};