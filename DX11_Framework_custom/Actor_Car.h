#pragma once
#include "Actor_Dynamic.h"
class Actor_Car : public Actor_Dynamic
{
public:
	Actor_Car() = default;
	Actor_Car(
		const ModelSpecs_W &worldSpecs,
		eTexture tex,
		const ModelSpecs_L &localSpecs,
		eModType modType = CUBE_TEXTURED ); 
	~Actor_Car() = default;


};

