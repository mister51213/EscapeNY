#include "Actor_Player.h"
#include "MathUtils.h"

using namespace DirectX;

Actor_Player::Actor_Player( const ModelSpecs_W & worldSpecs, eTexture tex, const ModelSpecs_L & localSpecs, eModType modType )
	:
	Actor_Dynamic( worldSpecs, tex, localSpecs, modType ) // base constructor
{}

void Actor_Player::GetInput( Input& pInput, int randI, float randF )
{
	Input::Event evnt = pInput.Peek();
	
	if ( pInput.IsKeyDown( VK_UP ) && !upPressLastFrame )
	{
		upPressLastFrame = true;
	}
	if ( pInput.IsKeyDown( VK_DOWN ) && !downPressLastFrame )
	{
		downPressLastFrame = true;
	}
	if ( pInput.IsKeyDown( VK_LEFT ) && !leftPressLastFrame )
	{
		leftPressLastFrame = true;
	}
	if ( pInput.IsKeyDown( VK_RIGHT ) && !rightPressLastFrame )
	{
		rightPressLastFrame = true;
	}

	if ( !pInput.IsKeyDown( VK_UP ) && upPressLastFrame )
	{
		upPressLastFrame = false;
		m_state = HOMING;
		m_target = { 0.f, 0.f, 200.f };
		m_initalPosition = GetPosition();
	}
	else if ( !pInput.IsKeyDown( VK_DOWN ) && downPressLastFrame )
	{
		downPressLastFrame = false;
		m_state = HOMING;
		m_target = { 0.f, 0.f, -200.f };
		m_initalPosition = GetPosition();
	}

	if ( !pInput.IsKeyDown( VK_RIGHT ) && rightPressLastFrame )
	{
		rightPressLastFrame = false;
		m_state = HOMING;
		m_target = { 200.f, 0.f, 0.0f };
		m_initalPosition = GetPosition();
	}
	else if ( !pInput.IsKeyDown( VK_LEFT ) && leftPressLastFrame )
	{
		leftPressLastFrame = false;
		m_state = HOMING;
		m_target = { -200.f, 0.f, 0.0f };
		m_initalPosition = GetPosition();
	}

	m_worldSpecs.orientation.y += pInput.GetRelativeX();
	m_worldSpecs.orientation.x += pInput.GetRelativeY();
}

void Actor_Player::Update( float deltaT )
{
	switch ( m_state )
	{
		case STILL:
			return;
		case FALLING:
		{

		}
		case HOMING:
		{
			ChaseTarget( deltaT ); // TODO: change this to non-default parameter
			break;
		}
		case CONST_MOVE:
		{
			Move( { 1.0f, 1.0f, 1.0f } );
			break;
		}
		default:
			return;
	}
}

// TODO: DEFUNCT
void Actor_Player::Move( const XMFLOAT3 & offset )
{
	m_worldSpecs.position += offset;
}

void Actor_Player::Rotate( const XMFLOAT3 & rotation )
{
	m_worldSpecs.orientation += rotation;
}
