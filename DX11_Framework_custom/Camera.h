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
        return m_Rotation;
    }
    void SetPosition(const XMFLOAT3 &Position)
    {
        m_Position = Position;
    }
    void SetRotation(const XMFLOAT3 &Rotation)
    {
        m_Rotation = Rotation;
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

    void GetInput(std::shared_ptr<Input> pInput)
    {
        const float time = 1.f;
        const float camSpeed = 2.f;
        float displacement = time*camSpeed;

        if (pInput->IsKeyDown('D'))
        {
            Strafe(displacement);
        }
        if (pInput->IsKeyDown('A'))
        {
            Strafe(-displacement);
        }
        if (pInput->IsKeyDown('W'))
        {
            Walk(displacement);
        }
        if (pInput->IsKeyDown('S'))
        {
            Walk(-displacement);
        }
    }

    void GetMouseInput(std::shared_ptr<Input> pInput)
    {
    // Get "angle" traveled by mouse since last position
    float angleX = XMConvertToRadians(
        0.25f*static_cast<float>(pInput->m_LastMousePos.x - m_LastMousePos.x));
    float angleY = XMConvertToRadians(
        0.25f*static_cast<float>(pInput->m_LastMousePos.y - m_LastMousePos.y));

    // Store current mouse position
    m_LastMousePos.x = pInput->m_LastMousePos.x;
    m_LastMousePos.y = pInput->m_LastMousePos.y;

    // Re-rotate camera based on new position of mouse
    Pitch(angleX);
    Yaw(angleY);
    }

    // ROTATE camera on the X axis
    void Pitch(float angle)
    {
        XMMATRIX RotationM = XMMatrixRotationAxis(XMLoadFloat3(&m_RightDir), angle);
        XMStoreFloat3(&m_UpDir, XMVector3TransformNormal(XMLoadFloat3(&m_UpDir), RotationM));
        XMStoreFloat3(&m_LookDir, XMVector3TransformNormal(XMLoadFloat3(&m_LookDir), RotationM));
    }

    // ROTATE camera on the Y axis
    void Yaw(float angle)
    {
        XMFLOAT3 upReset{ 0.f,1.f,0.f }; // Reset up vector to avoid tilt
        XMMATRIX RotationM = XMMatrixRotationAxis(XMLoadFloat3(&upReset), angle);
        XMStoreFloat3(&m_RightDir, XMVector3TransformNormal(XMLoadFloat3(&m_RightDir), RotationM));
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
        //// Load rotation XMFLOAT3 into a vector after converting it to radians
        //XMVECTOR pos = XMLoadFloat3(&(ConvertToRadians(m_Position)));
        //// Load up and right into vectors & convert to radians
        //XMVECTOR up = XMLoadFloat3(&(ConvertToRadians(m_UpDir)));
        //XMVECTOR right = XMLoadFloat3(&(ConvertToRadians(m_UpDir)));
        //// Rotate up and right vectors
        //XMVECTOR rotation = XMLoadFloat3(&(ConvertToRadians(m_Rotation)));
        //XMVECTOR upR = up/**rotation*/;
        //XMVECTOR rightR = right/**rotation*/;
        //// Cross product of rotated vectors
        //XMVECTOR fwdR = XMVector3Cross(upR, rightR);
        //// Calculate view matrix
        //m_ViewMatrix = XMMatrixLookAtLH(pos, fwdR, upR);

        // OLD WAY OF RENDERING
                // Load the rotation and make radian vectors.
    XMVECTOR rotationVector = XMLoadFloat3(&m_Rotation);
        // Setup the vector that points upwards.
    XMVECTOR upVector = XMVectorSet(0.f, 1.f, 0.f, 0.f);
        // Load the position into an XMVECTOR structure.
    XMVECTOR positionVector = XMLoadFloat3(&m_Position);
        // Setup where the camera is looking by default.
    //XMVECTOR lookAtVector = XMVectorSet(0.f, 0.f, 1.f, 0.f);
    XMVECTOR lookAtVector = XMLoadFloat3(&m_LookDir);
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

private:
    XMFLOAT3 m_Position = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 m_Rotation = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 m_RightDir = { 1.0f, 0.0f, 0.0f };
    XMFLOAT3 m_UpDir = { 0.0f, 1.0f, 0.0f };
    XMFLOAT3 m_LookDir = { 0.0f, 0.0f, 1.0f };
    XMMATRIX m_ViewMatrix, m_ProjectionMatrix, m_OrthoMatrix;;
    XMFLOAT3 m_LastMousePos = { 0.f,0.f,0.f };
};