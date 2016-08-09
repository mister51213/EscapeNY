#pragma once
#include "Includes.h"
#include "Utilities.h"

void Render() {
        // Load the rotation and make radian vectors.
    XMVECTOR rotationVector = XMLoadFloat3(&m_Rotation);
        // Setup the vector that points upwards.
    XMVECTOR upVector = XMVectorSet(0.f, 1.f, 0.f, 0.f);
        // Load the position into an XMVECTOR structure.
    XMVECTOR positionVector = XMLoadFloat3(&m_Position);
        // Setup where the camera is looking by default.
    XMVECTOR lookAtVector = XMVectorSet(0.f, 0.f, 1.f, 0.f);
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

    XMFLOAT3 m_Position = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 m_Rotation = { 0.0f, 0.0f, 0.0f };
    XMFLOAT3 m_RightDir = { 1.0f, 0.0f, 0.0f };
    XMFLOAT3 m_UpDir = { 0.0f, 1.0f, 0.0f };
    XMFLOAT3 m_LookDir = { 0.0f, 0.0f, 1.0f };
    XMMATRIX m_ViewMatrix, m_ProjectionMatrix, m_OrthoMatrix;;
    XMFLOAT3 m_LastMousePos = { 0.f,0.f,0.f };