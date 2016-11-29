#pragma once
#include "Actor.h"
class Actor_Scenery : public Actor
{
public:
	Actor_Scenery() = default;
	Actor_Scenery(
		const ModelSpecs_W &worldSpecs,
		eTexture tex,
		const ModelSpecs_L &localSpecs,
		eModType modType = CUBE_TEXTURED );
	~Actor_Scenery() = default;

};

