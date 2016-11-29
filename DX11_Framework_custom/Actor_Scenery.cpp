#include "Actor_Scenery.h"

Actor_Scenery::Actor_Scenery( 
	const ModelSpecs_W & worldSpecs, eTexture tex, 
	const ModelSpecs_L & localSpecs, eModType modType )
	:
	Actor( worldSpecs, tex, localSpecs, modType )
{}
