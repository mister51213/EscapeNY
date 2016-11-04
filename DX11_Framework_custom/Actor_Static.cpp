#include "Actor_Static.h"

Actor_Static::Actor_Static( 
	const ModelSpecs_W & worldSpecs, eTexture tex, 
	const ModelSpecs_L & localSpecs, eModType modType )
	:
	Actor( worldSpecs, tex, localSpecs, modType )
{}
