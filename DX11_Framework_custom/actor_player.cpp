#include "actor_player.h"

Actor_Player::Actor_Player( const ModelSpecs_W & worldSpecs, eTexture tex, const ModelSpecs_L & localSpecs, eModType modType )
	:
	Actor( worldSpecs, tex, localSpecs, modType ) // base constructor
{
}