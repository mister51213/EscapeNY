#include "Actor_Player.h"
#include "MathUtils.h"

using namespace DirectX;

Actor_Player::Actor_Player( const ModelSpecs_W & worldSpecs, eTexture tex, const ModelSpecs_L & localSpecs, eModType modType )
	:
	Actor_Dynamic( worldSpecs, tex, localSpecs, modType ) // base constructor
{
	// Set in constructor, and can only be accessed but not changed thereafter
	m_isMovable = true;
}

void Actor_Player::GetInput( const Input& pInput, int randI, float randF )
{
		float x = static_cast<float>( rand() % 800 - 400 );
   		float y = static_cast<float>( rand() % 800 - 400 );
        float z = static_cast<float>( rand() % 800 - 400 );

	if ( pInput.IsKeyDown( VK_RIGHT ) )
	{
			m_state = HOMING;
			ResetPIDParams( { x, y, 0.0f } );
	}
	else if ( pInput.IsKeyDown( VK_LEFT ) )
	{
			m_state = HOMING;
			ResetPIDParams( { -x, y, 0.0f } );
	}
	if ( pInput.IsKeyDown( VK_UP ) )
	{
			m_state = HOMING;
			ResetPIDParams( { 0.f, y, z } );
	}
	else if ( pInput.IsKeyDown( VK_DOWN ) )
	{
			m_state = HOMING;
			ResetPIDParams({ 0.f, y, -z });
	}
	if ( pInput.IsKeyDown( VK_SPACE ) )
	{
			m_state = HOMING;
			ResetPIDParams({ 0.f, 0.f, 0.f });
	}



	//if ( pInput.IsKeyDown( VK_RIGHT ) )
	//{
	//		m_state = HOMING;
	//		ResetPIDParams( { 200.f, 0.f, 0.0f } );
	//}
	//else if ( pInput.IsKeyDown( VK_LEFT ) )
	//{
	//		m_state = HOMING;
	//		ResetPIDParams( { -200.f, 0.f, 0.0f } );
	//}
	//if ( pInput.IsKeyDown( VK_UP ) )
	//{
	//		m_state = HOMING;
	//		ResetPIDParams( { 0.f, 0.f, 200.f } );
	//}
	//else if ( pInput.IsKeyDown( VK_DOWN ) )
	//{
	//		m_state = HOMING;
	//		ResetPIDParams({ 0.f, 0.f, -200.f });
	//}

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
