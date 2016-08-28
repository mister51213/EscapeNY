#include "Actor_Light.h"

Actor_Light::Actor_Light()
{
}

void Actor_Light::GetKeyInput_Light(const Input & pInput)
{
    Camera::GetInput(pInput);
    m_spotLight.SetDirection(m_Orientation);
    m_spotLight.SetPosition(m_Position);
}

void Actor_Light::GetMouseInput_Light(const Input & pInput)
{
    Camera::GetMouseInput(pInput);
    m_spotLight.SetDirection(m_Orientation);
    m_spotLight.SetPosition(m_Position);
}

Actor_Light::~Actor_Light()
{
}

