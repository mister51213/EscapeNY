#pragma once

#include "Texture.h"
#include "PrimitiveMaker.h"

class TexturedPlane
{
	struct VertexBufferType
	{

	};

public:
	TexturedPlane();
	~TexturedPlane();

	bool Initialize( const Texture & Tex, const PrimitiveMaker &PrimMaker );

private:
	Texture m_Texture;

};

