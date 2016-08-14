#include "Actor.h"

Actor::Actor( const ModelSpecs_W & worldSpecs, eTexture tex, const ModelSpecs_L & localSpecs, eModType modType )
	:
	m_worldSpecs( worldSpecs ),
	m_localSpecs( localSpecs ),
	m_texIndex( tex ),
	m_modType( modType )
{
}