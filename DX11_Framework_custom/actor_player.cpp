#include "Actor_Player.h"

Actor_Player::Actor_Player( const ModelSpecs_W & worldSpecs, eTexture tex, const ModelSpecs_L & localSpecs, eModType modType )
	:
	Actor( worldSpecs, tex, localSpecs, modType ) // base constructor
{}

void Actor_Player::GetInput(const Input& pInput, int randI, float randF)
    {
        if (pInput.IsKeyDown(VK_RIGHT))
        {
            //m_state = Move_PID;
            m_target += { 3.f, 0.f, 0.0f };
            //Move({ .6f, 0.f, 0.f });
        }
        if (pInput.IsKeyDown(VK_LEFT))
        {
//            m_state = Move_PID;
            m_target += { -3.f, 0.f, 0.0f };
            //Move({ -.6f, 0.f, 0.f });
        }
        if (pInput.IsKeyDown(VK_UP))
        {
//            m_state = Move_PID;
            m_target += { 0.f, 0.f, 3.0f };            
           //Move({ 0.f, 0.f, .6f });
        }
        if (pInput.IsKeyDown(VK_DOWN))
        {
//            m_state = Move_PID;
            m_target += { 0.f, 0.f, -3.0f };
            //Move({ 0.f, 0.f, -.6f });
        }

        m_worldSpecs.orientation.y += pInput.GetRelativeX();
		m_worldSpecs.orientation.x += pInput.GetRelativeY();
    } 