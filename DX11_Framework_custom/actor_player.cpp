#include "Actor_Player.h"
#include "MathUtils.h"

using namespace DirectX;

Actor_Player::Actor_Player( const ModelSpecs_W & worldSpecs, eTexture tex, const ModelSpecs_L & localSpecs, eModType modType )
	:
	Actor_Dynamic( worldSpecs, tex, localSpecs, modType ) // base constructor
{}

void Actor_Player::GetInput(const Input& pInput, int randI, float randF)
    {
        if (pInput.IsKeyDown(VK_RIGHT))
        {
            m_state = Move_PID;
            m_attributes.target = { 60.f, 0.f, 0.0f };
            //Move({ .6f, 0.f, 0.f });
        }
        if (pInput.IsKeyDown(VK_LEFT))
        {
            m_state = Move_PID;
            m_attributes.target = { -60.f, 0.f, 0.0f };
            //Move({ -.6f, 0.f, 0.f });
        }
        if (pInput.IsKeyDown(VK_UP))
        {
            m_state = Move_PID;
            m_attributes.target = { 0.f, 0.f, 60.0f };            
           //Move({ 0.f, 0.f, .6f });
        }
        if (pInput.IsKeyDown(VK_DOWN))
        {
            m_state = Move_PID;
            m_attributes.target = { 0.f, 0.f, -60.0f };
            //Move({ 0.f, 0.f, -.6f });
        }

        m_worldSpecs.orientation.y += pInput.GetRelativeX();
		m_worldSpecs.orientation.x += pInput.GetRelativeY();
    }

	void Actor_Player::Update(float deltaT) 
{
        switch (m_state)
        {
        case Stationary:
            return;
        case Falling:
            {

            }
        case Move_PID:
            {
                MovePID(deltaT); // TODO: change this to non-default parameter
				break;
            }
		case Moving:
            {
				Move( { 1.0f,1.0f,1.0f } );
				break;
            }
        default:
            return;
        }
}


void Actor_Player::Move( const XMFLOAT3 & offset )
{
	m_worldSpecs.position += offset;
}

void Actor_Player::Rotate( const XMFLOAT3 & rotation )
{
	m_worldSpecs.orientation += rotation;
}
