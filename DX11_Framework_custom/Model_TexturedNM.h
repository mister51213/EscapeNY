#pragma once
#include "Model.h"
class Model_TexturedNM :
	public Model
{
public:
	Model_TexturedNM();
	~Model_TexturedNM();

	bool Initialize( const PrimitiveFactory &PrimMaker, const Graphics &Gfx ) override;
private:
	
};

