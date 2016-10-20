#include "Actor_NPC.h"
#include "MathUtils.h"

Actor_NPC::Actor_NPC( const ModelSpecs_W & worldSpecs, eTexture tex, const ModelSpecs_L & localSpecs, eModType mod ) :
	Actor( worldSpecs, tex, localSpecs, mod ) // base constructor
{
}

void Actor_NPC::GetInput( const Input & pInput, int randI, float randF )
{
	//srand( static_cast<unsigned int>( time( nullptr ) ) );
	//int dieRoll = rand() % 3;
	//float randNum = (rand()%50)/10.0f; // get random float from 0~10.0

	if( pInput.IsKeyDown( VK_RIGHT ) )
	{
		//if(randI == 0)
		//Move({ randF, randF, randF });
		//else
		Move( { .5f, 0.f, 0.f } );
	}
	if( pInput.IsKeyDown( VK_LEFT ) )
	{
		//if(randI == 1)
		//    Move({ -randF, -randF, -randF });
		//else
		Move( { -.5f, 0.f, 0.f } );
	}

	if( pInput.IsKeyDown( VK_UP ) )
	{
		//if(randI == 2)
		//Move({ randF, randF, randF });
		//else
		Move( { 0.f, 0.f, .5f } );
	}
	if( pInput.IsKeyDown( VK_DOWN ) )
	{
		//if(randI == 3)
		//Move({ -randF, -randF, -randF });
		//else
		Move( { 0.f, 0.f, -.5f } );
	}

	if( pInput.IsKeyDown( VK_SPACE ) )
	{
		// Rotate({ 1.f,1.f,1.f});
		Rotate( { 0.f, 1.f, 0.f } );
	}

	if( pInput.IsKeyDown( VK_CONTROL ) )
	{
		//Rotate({ -1.f,-1.f,-1.f });
		Rotate( { 0.f, -1.f, 0.f } );
	}
}

void Actor_NPC::Move( const XMFLOAT3 & offset )
{
	m_worldSpecs.position += offset;
}

void Actor_NPC::Rotate( const XMFLOAT3 & rotation )
{
	m_worldSpecs.orientation += rotation;
}

