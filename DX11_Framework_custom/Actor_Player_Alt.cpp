#include "Actor_Player_Alt.h"
#include <math.h>


Actor_Player_Alt::Actor_Player_Alt()
	:
	m_speed(0.2f)
{
}

Actor_Player_Alt::Actor_Player_Alt( 
	const ModelSpecs_W &worldSpecs,
	eTexture tex,
	const ModelSpecs_L &localSpecs,
	eModType modType )
	:
	Actor( worldSpecs, tex, localSpecs, modType ),
	m_speed(0.2f)
{
}


Actor_Player_Alt::~Actor_Player_Alt()
{
}

void Actor_Player_Alt::SetPosition( const DirectX::XMFLOAT3 & Position )
{
	m_worldSpecs.position = Position;
}

void Actor_Player_Alt::GetInput( const Input& pInput, int randI, float randF )
{
	// Initialize the values to be used for direction
	float x = 0.f, z = 0.f;

	// Only eight directions user can point when using keybaord for 
	// input; 
	// north, northeast, east, southeast, 
	// south, southwest, west and northwest
	// Set X and Z to a constant 1 or -1 to use for direction
	// calculations
	if( pInput.IsKeyDown( VK_UP ) )
	{
		z = 1.f;
	}
	else if( pInput.IsKeyDown( VK_DOWN ) )
	{
		z = -1.f;
	}

	if( pInput.IsKeyDown( VK_LEFT ) )
	{
		x = -1.f;
	}
	else if( pInput.IsKeyDown( VK_RIGHT ) )
	{
		x = 1.f;
	}
		
	// rx and rz are used as result of multiplying recipricol magnitude
	// by the x and z values as mentioned below
	if( x == 0.f && z == 0.f )
	{
		return;
	}
	float recipLength = 1.f / sqrtf( pow( x, 2 ) + pow( z, 2 ) );
	float rx = x * recipLength;
	float rz = z * recipLength;
		
	// Figure out how to convert vector direction to angle in degrees
	bool posResult = ( ( rx > 0.f && rz > 0.f ) );
	bool negResult = ( ( rx < 0.f && rz < 0.f ) );
	float yRotation = g_degree * 
		( posResult && negResult ? asin( rx ) : -( fabsf( asin( rx ) ) ) );
		
	
	m_worldSpecs.orientation.y = yRotation;
	
	// Use the vector direction and speed to move player to new position
	m_worldSpecs.position.x += ( rx * m_speed );
	m_worldSpecs.position.z += ( rz * m_speed );
}
