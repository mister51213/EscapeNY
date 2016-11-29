#pragma once
#include "Actor_Dynamic.h"
class Actor_Stationary : public Actor_Dynamic
{
public:
	Actor_Stationary() = default;
	Actor_Stationary(
	const ModelSpecs_W & worldSpecs, 
	eTexture tex, 
	const ModelSpecs_L & localSpecs, 
	eModType mod = CUBE_TEXTURED);
	~Actor_Stationary();
};

