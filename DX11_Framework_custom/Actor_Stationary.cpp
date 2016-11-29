#include "Actor_Stationary.h"

Actor_Stationary::Actor_Stationary( 
	const ModelSpecs_W & worldSpecs, 
	eTexture tex, 
	const ModelSpecs_L & localSpecs, 
	eModType mod ) :
	Actor_Dynamic( worldSpecs, tex, localSpecs, mod ) // base constructor
{
	// Set in constructor, and can only be accessed but not changed thereafter
	m_isMovable = false;
}

Actor_Stationary::~Actor_Stationary()
{}
