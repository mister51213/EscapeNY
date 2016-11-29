#pragma once

#include "Actor.h"

class Actor_Static : public Actor
{
public:
	Actor_Static() = default;
	Actor_Static(
		const ModelSpecs_W &worldSpecs,
		eTexture tex,
		const ModelSpecs_L &localSpecs,
		eModType modType = CUBE_TEXTURED );
	~Actor_Static() = default;

};

