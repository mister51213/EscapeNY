#pragma once

#include "Model.h"

class Model_TexturedNM :
	public Model
{
public:
	bool Initialize( const PrimitiveFactory &PrimMaker, const Graphics &Gfx ) override;
	
};

