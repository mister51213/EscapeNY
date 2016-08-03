#pragma once

#include "Texture.h"
#include "PrimitiveFactory.h"

class TexturedPlane
{
	struct VertexBufferType
	{

	};

public:
	TexturedPlane();
	~TexturedPlane();

	bool Initialize( const Texture & Tex, const PrimitiveFactory &PrimMaker );

private:
	Texture m_Texture;

};

