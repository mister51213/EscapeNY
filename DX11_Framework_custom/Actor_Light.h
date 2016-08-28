#pragma once
#include "Actor.h"
#include "Light_Spot.h"
#include "Camera.h"

class Actor_Light :
    public Actor, public Camera
{
public:
    Actor_Light();
    void GetKeyInput_Light(const Input & pInput);
    void GetMouseInput_Light(const Input & pInput);
    ~Actor_Light();

    // NOTE: MUST CALL PARENT INITIALIZE FIRST!
    void InitSpotLight()
    {
        m_spotLight.SetDirection(m_Orientation);
        m_spotLight.SetPosition(m_Position);
        m_spotLight.SetColor(1.0f, 1.0f, 1.0f);
    }

    ILight* GetLight()
    {
        // transform the direction first
        //XMVECTOR posV = XMLoadFloat3(&m_spotLight.GetPosition());
        //XMVECTOR rotV = XMLoadFloat3(&m_spotLight.GetDirection());

        XMVECTOR posV = XMLoadFloat3(&Camera::GetPosition());
        XMVECTOR rotV = XMLoadFloat3(&Camera::GetRotation());

        // Convert rotV vector to radians
        rotV = ConvertToRadians(rotV);

        // Build view matrix from position and rotation
        XMMATRIX translationMat = XMMatrixTranslationFromVector(posV);
        XMMATRIX rotationMat = XMMatrixRotationRollPitchYawFromVector(rotV);

        XMVECTOR dirTrans = XMVector3Transform(rotV, rotationMat);
        dirTrans = XMVector3TransformCoord(dirTrans, translationMat);

        XMFLOAT3 dirFinal;
        XMStoreFloat3(&dirFinal, dirTrans);
        m_spotLight.SetDirection(dirFinal);

        return &m_spotLight;
    }

protected:
    Light_Spot m_spotLight;
};

