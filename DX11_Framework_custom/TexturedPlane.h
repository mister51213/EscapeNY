#pragma once

#include "Texture.h"

class PrimitiveFactory;

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

