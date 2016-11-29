#include "Actor_Player_Alt.h"
#include "MathUtils.h"
#include "Input.h"

Actor_Player_Alt::Actor_Player_Alt( 
	const ModelSpecs_W &worldSpecs,
	eTexture tex,
	Input *pInput,
	eModType modType )
	:
	Actor_Dynamic( worldSpecs, tex, ModelSpecs_L(), modType ),
	m_pInput(pInput),
	m_speed(0.2f)
{
	m_state = Actor_Dynamic::PLAYER_IDLE;
}

void Actor_Player_Alt::Update( float DeltaTime )
{
	switch ( m_state )
	{
		case Actor_Dynamic::PLAYER_IDLE:
			// Set idle animation/pose
			break;
		case Actor_Dynamic::PLAYER_WALKING:
			// Set walk animation
			Walk( DeltaTime );
			break;
		case Actor_Dynamic::PLAYER_PUSHING:
			// Set pushing animation
			Push( DeltaTime );
			break;
		case Actor_Dynamic::PLAYER_QUICKTIME:
			// Set panic animation
			QuickTime();
			break;
		case Actor_Dynamic::PLAYER_DROWNING:
			// Set flailing animation
			break;
		case Actor_Dynamic::PLAYER_PINNED:
			// Set dead animation/pose
			break;
	}
}

void Actor_Player_Alt::Walk( float DeltaTime )
{
	// Initialize the values to be used for direction
	bool keyIsPressed = false;	
	float minuend = 0.f, yRot = 0.f;
		
	const float z = [ & ]
	{		
		if ( m_pInput->IsKeyDown( VK_UP ) )
		{
			keyIsPressed = true;
			yRot = 90.f;
			return 1.f;
		}
		else if ( m_pInput->IsKeyDown( VK_DOWN ) )
		{
			keyIsPressed = true;
			yRot = -90.f;
			return -1.f;
		}
		return 0.f;
	}( );
	const float x = [ & ]
	{
		if ( m_pInput->IsKeyDown( VK_LEFT ) )
		{
			keyIsPressed = true;
			minuend = 180.f;
			return -1.f;
		}
		else if ( m_pInput->IsKeyDown( VK_RIGHT ) )
		{
			keyIsPressed = true;
			minuend = 90.f;
			return 1.f;
		}
		return 0.f;
	}( );

	if ( !keyIsPressed ) return;

	// rx and rz are used as result of multiplying recipricol magnitude
	// by the x and z values as mentioned below
	const float recipLength = 1.f / sqrtf( pow( x, 2 ) + pow( z, 2 ) );
	const float rx = x * recipLength;
	const float rz = z * recipLength;

	const float sign = std::signbit( yRot ) ? -1.f : 1.f;
	m_worldSpecs.orientation.y = ( minuend - ( abs( yRot ) * .5f ) ) * sign;

	// Use the vector direction and speed to move player to new position
	m_worldSpecs.position.x += ( rx * m_speed );
	m_worldSpecs.position.z += ( rz * m_speed );
}

void Actor_Player_Alt::Push( float DeltaTime )
{}

void Actor_Player_Alt::QuickTime()
{}
