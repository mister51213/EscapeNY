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

void Actor_Player_Alt::Update( const Input & UserInput )
{
	// Initialize the values to be used for direction
	float x = 0.f, z = 0.f;

	// Only eight directions user can point when using keybaord for 
	// input; 
	// north, northeast, east, southeast, 
	// south, southwest, west and northwest
	// Set X and Z to a constant 1 or -1 to use for direction
	// calculations
	if( UserInput.IsKeyDown( VK_UP ) )
	{
		z = 1.f;
	}
	else if( UserInput.IsKeyDown( VK_DOWN ) )
	{
		z = -1.f;
	}

	if( UserInput.IsKeyDown( VK_LEFT ) )
	{
		x = -1.f;
	}
	else if( UserInput.IsKeyDown( VK_RIGHT ) )
	{
		x = 1.f;
	}

	/////////////////////////////////////////////////////////////////////////
	// TODO: Maybe move to a global function 							   //
	// Maybe if this is something that will be used often, we should move  //
	// to a global function												   //
	/////////////////////////////////////////////////////////////////////////
	// rx and rz are used as result of multiplying recipricol magnitude
	// by the x and z values as mentioned below
	float rx = 0.f;
	float rz = 0.f;

	// Made a function in Utilities.h to convert the keyboard input
	// to a rotation in Y axis.  
	float yRotation = CalculateYRotation( x, z, rx, rz );
	
	// If function returns -0.f( Yes, -0.f is a thing ), no calculations 
	// were done so skip assigning rotation to world specs member.
	// While it is a thing, while in float form, comparing 0.f and -0.f
	// are the same apparently, so casting to an int* then dereferencing
	// it, shows they are different values in memory.

	// Store -0.f 
	float nZero = -0.f;
	// Convert to int * and dereference to get the int value
	int negZero = *(int*)&nZero;
	// Convert yRotation to int * and dereference to get the int value
	int fConversion = *(int*)&yRotation;

	// Compare the converted values
	if( fConversion != negZero )
	{
		m_worldSpecs.orientation.y = yRotation;
	}

	// Use the vector direction and speed to move player to new position
	m_worldSpecs.position.x += ( rx * m_speed );
	m_worldSpecs.position.z += ( rz * m_speed );
}
